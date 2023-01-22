
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




int main(){
	// PCore core;
	// PTCPServer server(18);
	// server.bind("192.168.234.5",3309);
	// server.listen(13);
	// infoL("man Thread:" << std::this_thread::get_id());
	// server.connect(&server,&PTCPServer::connected,0x33,[](PTCPSocket *socket){
	// 		infoL("socket Thread:" << std::this_thread::get_id());
	// 		infoL("client:"<< socket->getIp());
	// 		socket->connect(socket,&PTCPSocket::readReady,0x78,[](PTCPSocket * socket){
	// 			infoL("socket Thread:" << std::this_thread::get_id());
	// 			 char buffer[512]={0};
	// 			int ret= socket->read(buffer,512);
	// 			buffer[ret]=0;
	// 			std::string msg(buffer);
	// 			std::cout << "data:" << msg;
				 
	// 		});
	// });

	// server.start();

	// core.exec();

	
	//std::array<int,10> tr;

	PByteArray array;
	char bufferR[13];
	const char * dpl="Hello,world,fing,hhh";
	const char *bigbuffer="QWERTYUIOPPASDFGHJKLZXCVBNM";

	array.push(bigbuffer,25);

	array.pop(bufferR,12);

	array.push(dpl,20);
	
	int leftSize=array.size();

	
	return 0;
}




