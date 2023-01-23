
#include <utility>
#include <memory>
#include <vector>
#include <iostream>
#include <functional>
#include <vector>
#include <list>
#include <queue>
#include <array>
#include "PObject.h"
#include "PThread.h"
#include "PTimer.h"
#include "PTCPSocket.h"
#include "PByteArray.hpp"

#ifdef _WINDWOS

#elif __MACH__
#include <unistd.h>

#endif

int main()
{
	PCore core;
	PTCPServer server(18);
	server.bind("192.168.234.5",3308);
	server.listen(13);
	infoL("man Thread:" << std::this_thread::get_id());
	server.connect(&server,&PTCPServer::connected,0x33,[](PTCPSocket *socket){
			infoL("socket Thread:" << std::this_thread::get_id());
			infoL("client:"<< socket->getIp());
			socket->connect(socket,&PTCPSocket::readReady,0x78,[](PTCPSocket * socket){
				//infoL("socket Thread:" << std::this_thread::get_id());
				 char buffer[512]={0};
				int ret= socket->read(buffer,512);
				buffer[ret]=0;
				std::string msg(buffer);
				std::cout << "data:" << msg;
				socket->send(buffer,ret);

			});
	});

	server.start();

	core.exec();

	// std::array<int,10> tr;

	// PByteArray array(5);
	// char bufferR[40]={};
	// const char * dpl="Hello,world,fing,hhh";
	// const char *bigbuffer="QWERTYUIOPPASDFGHJKLZXCVBNM";

	// std::string msg="IAM BIG BANG KE HHHHHH FUCK FUCK FUCK!";

	// array.push(bigbuffer,25);

	// array.pop(bufferR,12);

	// array.push(dpl,20);
	// int leftSize=array.size();
	// array.push_from_string(msg);

	// leftSize=array.size();

	// //array.pop(bufferR,40);
	// PByteArray dlp(array);
	// std::string ret_2=dlp.pop_to_string();
	// std::string ret=array.pop_to_string();

	// int ret = 0;
	// int listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// if (listen_fd < 0)
	// {
	// 	errorL("socket:" << strerror(errno));
		
	// }
	// int kqueue_handle = kqueue();
	// sockaddr_in serverAddr = {0};
	// serverAddr.sin_family = AF_INET;
	// serverAddr.sin_port = htons(3309);
	// inet_aton("192.168.234.5", &serverAddr.sin_addr);

	// ret = ::bind(listen_fd, (sockaddr *)&serverAddr, sizeof(serverAddr));
	// if (0 > ret)
	// {
	// 	errorL("bind:" << strerror(errno));
	// }
	// struct kevent listen_event;

	// ret = ::listen(listen_fd, 56);
	// if (ret < 0)
	// {
	// 	errorL("listen:" << strerror(errno));
		
	// }
	// struct kevent checklist[5];
	// EV_SET(&listen_event, listen_fd, EVFILT_READ | EVFILT_WRITE, EV_ADD, 0, 0, NULL);
	// ret = kevent(kqueue_handle, &listen_event, 1, NULL, 0, NULL);

	// ret = kevent(kqueue_handle, NULL, 0, checklist, 5, NULL);

	// struct kevent clientE;
	// sockaddr_in clientAddr;
	// socklen_t lent = sizeof(sockaddr_in);
	// int client_fd = ::accept(listen_fd, (sockaddr *)&clientAddr, &lent);
	// if (client_fd < 0)
	// {
	// 	errorL("accpet:" << strerror(errno));
	// }
	// EV_SET(&clientE, client_fd, EVFILT_READ | EVFILT_WRITE, EV_ADD, 0, 0, NULL);
	// ret = kevent(kqueue_handle, &clientE, 1, NULL, 0, NULL);
	// EV_SET(&clientE, client_fd, EVFILT_WRITE, EV_ENABLE, 0, 0, NULL);
	// ret = kevent(kqueue_handle, &clientE, 1, NULL, 0, NULL);
	return 0;
}
