#ifndef __WinTCPSocket_H_WINDOWS
#define __WinTCPSocket_H_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include "../PByteArray.hpp"
#include "../BLThread.h"
#include <WinSock2.h>


#define DataBuffSize 10240
enum io_type_m { IO_READ, IO_WRITE };

typedef struct {
	OVERLAPPED overlapped;
	WSABUF databuff;
	io_type_m io_type_;
	CHAR buffer[DataBuffSize];
} PER_IO_OPERATEION_DATA, * LPPER_IO_OPERATION_DATA;

namespace BL {
	class WinTCPServer;
    class IOCPObject;

	class WinTCPSocket : public Object {
	public:
		WinTCPSocket(IOCPObject* iocp_t);
		WinTCPSocket(const std::thread::id& id,SOCKET socket_t, IOCPObject * iocp_t, Object* parent = nullptr);
		~WinTCPSocket();


		std::string getIp();

		int getPort();


		void Slot_onIOEvent(void* socket, LPPER_IO_OPERATION_DATA pIodata, int readBytes);

		void Slot_onIOClose(void* socket, LPPER_IO_OPERATION_DATA pIodata, int readBytes);


		exegesis("connect about")

			void connectToHost(const std::string& ip, int port);


		exegesis("signals")
		signal_<WinTCPSocket*> readReady;

		signal_<WinTCPSocket*> disconnected;

		signal_<WinTCPSocket*> hostConnected;


		exegesis("read or write")
			void send(const char* data, int size);

		int read(char* data, int size);


		HANDLE complationPort;

	private:
		signal_<> writeReady;
	private:
		friend BL::WinTCPServer;
		struct sockaddr_in Socketaddr;

		void onWrite();
		std::mutex cache_lock;
		PER_IO_OPERATEION_DATA sendData;
		PER_IO_OPERATEION_DATA recaveData;
		PByteArray sendCache;
		PByteArray recavCache;
		SOCKET m_socket;
		IOCPObject* iocp = nullptr;
	};


	class IOCPObject :public Object {
	public:
		IOCPObject(int threadCount);

		~IOCPObject();
		void addIOEvent(void * FileHandle,void* args);

		void exit();

		exegesis("signal")

		signal_<void*, LPPER_IO_OPERATION_DATA , int  > IOEvent;

		signal_<void*, LPPER_IO_OPERATION_DATA, int> IOClosed;

	private:
		static void runIOCP(IOCPObject* iocp);

		HANDLE complationPort;

		std::atomic<bool> isRun = true;

		std::thread _th;


	};


	class WinTCPServer : public Object {
	public:
		WinTCPServer(IOCPObject * iocp_t, Object* parent = nullptr);

		~WinTCPServer();

		bool bind(const std::string& ip, int port);

		bool listen(int MaxListenSize);

		void exit();

		void start();

	private:
		

	private:
		void run();

	public:
		signal_<WinTCPSocket*> connected;

		signal_<WinTCPSocket*> errered;

	private:
		friend BL::WinTCPSocket;

		signal_<WinTCPSocket*> freeSocket;

		void onFreeSocketSlot(WinTCPSocket* socket);

	private:
		WinTCPSocket* listen_Socket = 0;
		IOCPObject* iocp = nullptr;
		Thread accptThread;
		std::atomic<bool> isRun = true;
	};
}
#endif
