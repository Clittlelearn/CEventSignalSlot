


#include <thread>
#include <tuple>
#include <type_traits>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#include "windows/BLWinTCPSocket.h"
#include <sstream>
#include <vector>




    std::string ArgToString(const char * value) {
        return  std::string(value);
    }
    
    std::string ArgToString(char * value) {
        return  std::string(value);
    }
    template<int N>
    std::string ArgToString(char(&s)[N]) {
        return  std::string(s);
    }
    template<int N>
    std::string ArgToString(const char(&s)[N]) {
       return  std::string(s);
    }

   

    template <typename T>
    std::string ArgToString(T value) {
        return  std::to_string(value);
    }

    template <typename T>
    void make_args_vect(std::vector<std::string>& vec,  T t) {
        vec.push_back(ArgToString(t));
    }
    template<typename T,typename ...Ts>
    void make_args_vect(std::vector<std::string>& vec,  T t, Ts...  ts) {
        vec.push_back(ArgToString(t));
        make_args_vect(vec, ts...);
    }




class String {
public:
    template<typename... Ts>
    static std::string Format(const std::string& str, const Ts &... args) {
        std::vector<std::string> allArgs;
        make_args_vect(allArgs, args...);
        return BuildFormatTarget(str,allArgs);
    }

   
private:
  static std::string  BuildFormatTarget(const std::string& str,const std::vector<std::string> & allarg) {
      int arg_s = 0;
      int index = 0;
      std::stringstream svl;
      for (; index < str.size(); index++) {
          if (str[index] == '%' &&
              index != str.size() - 1 && 
              str[index + 1] =='s'&&
              arg_s < allarg.size()) {
              svl << allarg[arg_s];
              index += 1;
              arg_s++;
              continue;
          }
          svl << str[index];
      }
      return svl.str();
   }
 

};






//你好世界

//你好世界
int main() {


    infoL("main Id:" << std::this_thread::get_id());
    BL::IOCPObject iocp(3);
	BL::WinTCPServer server(&iocp);
	server.bind("127.0.0.1", 3309);
	server.listen(4);
	server.connected.connect(&server, [](BL::WinTCPSocket* soc) {
		infoL("connected <---->" << soc->getIp() << ":" << soc->getPort());
        infoL("connected id:"<< std::this_thread::get_id());
		soc->readReady.connect(soc, [](BL::WinTCPSocket* socket) {
            infoL("read id:"<< std::this_thread::get_id());
			char buffer[1024] = { 0 };
			int ret=socket->read(buffer, 1024);
			infoL(buffer);
            
		});
        soc->disconnected.connect(soc,[](BL::WinTCPSocket * soc){
            infoL("disconnected <---->" << soc->getIp() << ":" << soc->getPort());
            delete soc;
        });
	});
	server.start();

   /* auto ret= String::Format("cplus \" plus is %s -> %s -> %s", "jdfsaasdf",34.0,567,"dkafsasdf");

    infoL(ret);*/

	return 0;
}
