#include "BLWinTCPSocket.h"
#include "../debug.h"
#include <WS2tcpip.h>

namespace BL {
	WinTCPSocket::WinTCPSocket(IOCPObject* iocp_t)
	{
		iocp = iocp_t;
		iocp->IOEvent.connect(this, this, &WinTCPSocket::Slot_onIOEvent);

		iocp->IOClosed.connect(this, this, &WinTCPSocket::Slot_onIOClose);
	}

	
	WinTCPSocket::WinTCPSocket(const std::thread::id& id, SOCKET socket_t, IOCPObject* iocp_t, Object* parent )
		: Object(parent) {
		m_socket = socket_t;
		recavCache = PByteArray(10240);
		sendCache = PByteArray(10240);
		
		iocp = iocp_t;

		iocp->IOEvent.connect(this, this, &WinTCPSocket::Slot_onIOEvent);

		iocp->IOClosed.connect(this, this, &WinTCPSocket::Slot_onIOClose);


		setObjectThread(id);
	}

	std::string WinTCPSocket::getIp() {
		char buff[20] = { 0 };
		return std::string(
			inet_ntop(AF_INET, (const void*)&Socketaddr.sin_addr, buff, 20));
	}

	int WinTCPSocket::getPort() { return ntohs(Socketaddr.sin_port); }

	void WinTCPSocket::connectToHost(const std::string& ip, int port)
	{

		
		m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_socket == INVALID_SOCKET) {
			errorL("socket function failed with error: " << WSAGetLastError());
			WSACleanup();
			return ;
		}

		Socketaddr.sin_family = AF_INET;
		Socketaddr.sin_port = htons(port);

		inet_pton(AF_INET, ip.c_str(), &Socketaddr.sin_addr.S_un);


		int iResult = connect(m_socket, (SOCKADDR*)&Socketaddr, sizeof(struct sockaddr_in));
		if (iResult == SOCKET_ERROR) {
			errorL("connect function failed with error: " << WSAGetLastError());
			iResult = closesocket(m_socket);
			if (iResult == SOCKET_ERROR)
				errorL("closesocket function failed with error: " <<  WSAGetLastError());
			WSACleanup();
			return;
		}




	}

	void WinTCPSocket::Slot_onIOEvent(void* socket, LPPER_IO_OPERATION_DATA pIodata, int readBytes)
	{
		WinTCPSocket* so = (WinTCPSocket*)socket;
		so->recavCache.push(pIodata->buffer, readBytes);
		so->readReady.emit(so);

		DWORD sendBytes = 0;
		DWORD recvBytes = 0;
		DWORD flags=0;


		ZeroMemory(&(pIodata->overlapped), sizeof(OVERLAPPED));
		pIodata->databuff.len = DataBuffSize;
		pIodata->databuff.buf = pIodata->buffer;

		int ret = WSARecv(so->m_socket, &(pIodata->databuff), 1,
			&recvBytes, &flags, &(pIodata->overlapped), NULL);
		if (ret == SOCKET_ERROR) {
			if (WSAGetLastError() != ERROR_IO_PENDING) {
				errorL("WSARecv() failed. Error:" << GetLastError());
				return;
			}
		}
	}


	void WinTCPSocket::Slot_onIOClose(void* socket, LPPER_IO_OPERATION_DATA pIodata, int readBytes)
	{
		WinTCPSocket* so = (WinTCPSocket*)socket;
		so->disconnected.emit(so);
	}

	WinTCPSocket::~WinTCPSocket() {}

	void WinTCPSocket::send(const char* data, int size) {
		std::unique_lock<std::mutex> locker(cache_lock);
		sendCache.push(data, size);
		writeReady.emit();
	}

	void WinTCPSocket::onWrite() {
		std::unique_lock<std::mutex> locker(cache_lock);
		DWORD sendBytes = 0;
		LPPER_IO_OPERATION_DATA pIoData = &sendData;
		int sendSize =
			(DataBuffSize > sendCache.size()) ? sendCache.size() : DataBuffSize;
		ZeroMemory(&(pIoData->overlapped), sizeof(OVERLAPPED));
		pIoData->databuff.buf = pIoData->buffer;
		pIoData->databuff.len = sendSize;
		sendCache.pop(pIoData->buffer, sendSize);



		if (WSASend(m_socket, &(pIoData->databuff), 1, &sendBytes, 0,
			&(pIoData->overlapped), NULL) == SOCKET_ERROR) {
			if (WSAGetLastError() != ERROR_IO_PENDING) {
				errorL("WSASend() failed. Error:");
				return;
			}
			else {
				infoL("WSASend() failed. io pending. ");
			}
		}

		if (sendCache.size() > 0) {
			writeReady.emit();
		}
	}

	int WinTCPSocket::read(char* data, int size) {
		int rsize = (recavCache.size() > size) ? size : recavCache.size();
		recavCache.pop(data, rsize);
		return rsize;
	}

	class WSASETUP {
	public:
		WSASETUP() {
			WSADATA wsaData;
			ret = WSAStartup(0x0202, &wsaData);
			if (ret != 0) {
				errorL("WSAStartup failed. Error:" << ret);
			}
		}
		int init() { return ret; }
		int ret;
	};

	static WSASETUP wsasetup;

	WinTCPServer::WinTCPServer(IOCPObject* iocp_t, Object* parent) {
		/*int ret = wsasetup.init();
		if (ret != 0) {
			errorL("laset error:" << GetLastError());
			return;
		}*/
		iocp = iocp_t;
		
		freeSocket.connect(this, this, &WinTCPServer::onFreeSocketSlot);
		accptThread.started.connect(&accptThread, this, &WinTCPServer::run);

	
	}



	WinTCPServer::~WinTCPServer() {}



	bool WinTCPServer::bind(const std::string& ip, int port) {
		SOCKET listenSocket =
			WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		if (listenSocket == INVALID_SOCKET) {
			errorL(" WSASocket( listenSocket ) failed. Error:" << GetLastError());
			return false;
		}
		listen_Socket =new WinTCPSocket(getWorkThreadLoopId(), listenSocket, iocp, nullptr);

		SOCKADDR_IN internetAddr;
		internetAddr.sin_family = AF_INET;
		internetAddr.sin_port = htons(port);

		inet_pton(AF_INET, ip.c_str(), &internetAddr.sin_addr.S_un);

		if (::bind(listenSocket, (PSOCKADDR)&internetAddr, sizeof(internetAddr)) ==
			SOCKET_ERROR) {
			errorL("Bind failed. Error:" << GetLastError());
			return false;
		}
		return true;
	}

	bool WinTCPServer::listen(int MaxListenSize) {
		if (::listen(listen_Socket->m_socket, MaxListenSize) == SOCKET_ERROR) {
			errorL("listen failed. Error:" << GetLastError());
			return false;
		}
		return true;
	}

	void WinTCPServer::exit() {}

	void WinTCPServer::start() {
		accptThread.start();
		infoL("main thread id:" << std::this_thread::get_id());
		exec();
	}


	

	void WinTCPServer::run() {

		debugL("runing .....");
		infoL("work thread id:" << std::this_thread::get_id());
		
		DWORD recvBytes;
		DWORD flags;

		SOCKET t_socket = NULL;

		WinTCPSocket* m_custom_socket = nullptr;

		sockaddr_in client_addr;
		int addrSize = sizeof(sockaddr_in);

		while (isRun) {

			t_socket = WSAAccept(listen_Socket->m_socket, (sockaddr*)&client_addr,
				&addrSize, NULL, 0);

			m_custom_socket =new WinTCPSocket(getWorkThreadLoopId(), t_socket, iocp, nullptr);

			m_custom_socket->Socketaddr = client_addr;

			if (t_socket == SOCKET_ERROR) {
				errorL("WSAAccept failed. Error:" << GetLastError());
				return;
			}
			infoL("listen....");

			iocp->addIOEvent((HANDLE)t_socket, m_custom_socket);


			LPPER_IO_OPERATION_DATA pIoData = &m_custom_socket->recaveData;
			if (pIoData == NULL) {
				errorL("GlobalAlloc( IoData ) failed. Error:" << GetLastError());
				return;
			}

			ZeroMemory(&(pIoData->overlapped), sizeof(pIoData->overlapped));
			pIoData->databuff.len = DataBuffSize;
			pIoData->databuff.buf = pIoData->buffer;
			pIoData->io_type_ = IO_READ;
			flags = 0;

			if (WSARecv(t_socket, &(pIoData->databuff), 1, &recvBytes, &flags,
				&(pIoData->overlapped), NULL) == SOCKET_ERROR) {
				if (WSAGetLastError() != ERROR_IO_PENDING) {
					errorL("WSARecv() failed. Error:" << GetLastError());
					return;
				}
			}
			connected.emit(m_custom_socket);
		}
	}

	void WinTCPServer::onFreeSocketSlot(WinTCPSocket* socket) {
		infoL("delete");
		delete socket;
	}

	IOCPObject::IOCPObject(int threadCount)
	{
		int ret = wsasetup.init();
		if (ret != 0) {
			errorL("laset error:" << GetLastError());
			return;
		}
		
		complationPort =
			CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, threadCount);
		int er = GetLastError();
		if (complationPort == NULL || er != 0) {
			errorL("CreateIoCompletionPort failed. Error:" << GetLastError());
			return;
		}

		_th = std::thread(runIOCP,this);

	}

	IOCPObject::~IOCPObject()
	{
		WSACleanup();
	}

	void IOCPObject::addIOEvent(void* FileHandle, void* args)
	{
		if (CreateIoCompletionPort((HANDLE)FileHandle, complationPort,
			(ULONG_PTR)args, 0) == NULL) {
			errorL("CreateIoCompletionPort failed. Error:" << GetLastError());
			return;
		}
	}

	void IOCPObject::exit()
	{
		isRun = false;
	}

	void IOCPObject::runIOCP(IOCPObject* iocp)
	{
		HANDLE complationPort = (HANDLE)iocp->complationPort;
		DWORD bytesTransferred;
		void* complationkey = NULL;
		LPPER_IO_OPERATION_DATA pIoData = NULL;

		while (iocp->isRun) {
			if (GetQueuedCompletionStatus(
				complationPort, &bytesTransferred, (PULONG_PTR)&complationkey,
				(LPOVERLAPPED*)&pIoData, INFINITE) == 0) {

				if (complationkey == nullptr) {
					int er = GetLastError();
					if (er != 0) {
						errorL("error :" << er);
						return;
					}
					continue;
				}
				iocp->IOClosed.emit(complationkey,pIoData, bytesTransferred);
				continue;
			}

			if (bytesTransferred == 0 || iocp->isRun == false) {
				infoL("client has closed");
				iocp->IOClosed.emit(complationkey, pIoData, bytesTransferred);
				continue;
			}
			if (pIoData->io_type_ == IO_READ) {
				iocp->IOEvent.emit(complationkey, pIoData, bytesTransferred);
			}
		}
	}

} // namespace BL
