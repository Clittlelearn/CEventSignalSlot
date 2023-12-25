#ifndef __ER_object_H_
#define __ER_object_H_
#include <queue>
#include <string>
#include <vector>
#include <tuple>
#include "../BLThread.h"
namespace BL {

    class ErrorMsg{
        public:
            ErrorMsg();
            void setError(int code,const std::string& er);
            void Error(int code,const std::string& er);
            std::pair<int,std::string> GetError() ;
    private:

            std::string LastError;
            int ErrorCode;
            bool isClean = true;
    };
    static void Error(int code, const std::string& msg);
    static void SetLastError(int code, const std::string& msg);
    static std::pair<int, std::string> GetLasetError();



    class GlobalError :public Thread{
        public:
            GlobalError();

            void run() override;
        static  signal_<ErrorMsg&> errorSome;

    private:
      
    };
}


#endif