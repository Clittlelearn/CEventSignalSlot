
#include <utility>
#include <memory>
#include <vector>
#include <iostream>
#include <functional>
#include <vector>
#include <list>
#include <queue>
#include "PObject.h"
#include "PThread.h"
#include "PTimer.h"
#include "PTCPSocket.h"

#ifdef _WINDWOS

#elif __MACH__
#include <unistd.h>

#endif




int main(){
	PTCPServer server(18);
	server.bind("192.168.234.5",3309);
	server.listen(13);
	server.connect(&server,&PTCPServer::connected,0x33,[](PTCPSocket *socket){
			
	});
	
	return 0;
}




