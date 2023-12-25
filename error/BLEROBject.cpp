#include "BLERObject.h"
#include "BLERObject.h"
#include "BLERObject.h"
#include "BLERObject.h"
#include "BLERObject.h"
#include <map>
#include <thread>
namespace BL
{
   static thread_local ErrorMsg error_n;
    
    void Error(int code,const std::string & msg)
    {
        error_n.Error(code, msg);
    }

    void SetLastError( int code,const std::string& msg)
    {
        error_n.setError(code, msg);
    }

    std::pair<int, std::string> GetLasetError()
    {
        return error_n.GetError();
    }

   
    ErrorMsg::ErrorMsg()
    {
        ErrorCode = 0;
        LastError = "su";
    }

    void ErrorMsg::setError(int code, const std::string& er)
    {
        LastError = er;
        isClean = false;
        ErrorCode = code;

    }
    void ErrorMsg::Error(int code, const std::string& er)
    {
        if (isClean == true) {
            isClean = false;
            LastError = er;
            ErrorCode = code;

        }
    }

    std::pair<int, std::string> ErrorMsg::GetError()
    {
        isClean = true;
        return { ErrorCode ,LastError };
    }
   
   
    GlobalError::GlobalError()
    {
    }
    void GlobalError::run()
    {

    }
}