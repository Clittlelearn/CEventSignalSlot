#ifndef __DEBUG__L
#define __DEBUG__L

#include <atomic>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <fstream>
#define RED_t "\033[31m"
#define YELLOW_t "\033[33m"
#define GREEN_t "\033[32m"
#define WRITE "\033[0m"

#include <sstream>
static std::string ArgToString(const char* value) {
    return  std::string(value);
}

static std::string ArgToString(char* value) {
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

static std::string ArgToString(const std::string & str){
    return str;
}


template <typename T>
std::string ArgToString(const T& value) {
    return  std::to_string(value);
}

template <typename T>
void make_args_vect(std::vector<std::string>& vec,const T& t) {
    vec.push_back(ArgToString(t));
}
template<typename T, typename ...Ts>
void make_args_vect(std::vector<std::string>& vec, const T& t, const Ts &...  ts) {
    vec.push_back(ArgToString(t));
    make_args_vect(vec, ts...);
}

static void make_args_vect(std::vector<std::string>& vec){

}

std::string print_trace(void);


class Sutil {
public:
    template<typename T,typename... Ts>
    static std::string Format(const T& str, const Ts &... args) {
        std::vector<std::string> allArgs;
        make_args_vect(allArgs, args...);
        return BuildFormatTarget(ArgToString(str), allArgs);
    }


private:
    static std::string  BuildFormatTarget(const std::string& str, const std::vector<std::string>& allarg) {
        int arg_s = 0;
        int index = 0;
        std::stringstream svl;
        for (; index < str.size(); index++) {
            if (str[index] == '%' &&
                index != str.size() - 1 &&
                str[index + 1] == 's' &&
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


class LogHandler{
public:
    using ptr=std::shared_ptr<LogHandler>;
    LogHandler(const std::string & name){
        out.open(name);
    }
    ~LogHandler(){
        out.close();
    }
    void Log(const std::string & msg){
        std::unique_lock<std::mutex> locker(lock);
        out << msg;
        out.flush();
    }
    void SetLogName(const std::string & name){
        if(out.is_open()){
            out.close();
        }
        out.open(name);
    }
private:
    std::ofstream out;
    std::mutex lock;
};

#define BASECOLOER "\033[0m"


LogHandler::ptr GetLogInstance(const std::string & name);

void SetGLogName(const std::string & name);

void GLog(const std::string & meg);

#define LOGINSTANCE(name)
    //static LogHandler::ptr _log_instance_obj=GetLogInstance(name);

// #define SETLOGNAME(name)\ 
//     _log_instance_obj->SetLogName(name);

#define errorL(msg,...) \
    GLog(Sutil::Format("\nerror:[%s:%s]:",__FILE__,__LINE__)+Sutil::Format(msg,##__VA_ARGS__));
#define debugL(msg,...) \
	GLog(Sutil::Format("\ndebug:[%s:%s]:",__FILE__,__LINE__)+Sutil::Format(msg,##__VA_ARGS__));
#define infoL(msg,...) \
    GLog(Sutil::Format("\ninfo:[%s:%s]:",__FILE__,__LINE__)+Sutil::Format(msg,##__VA_ARGS__));



#define errorS(msg,...) \
	std::cout << RED_t <<"Error:["<< __FILE__  << ":"<< __LINE__ << "]:"<< Sutil::Format(msg, ##__VA_ARGS__) <<BASECOLOER<< std::endl;
#define debugS(msg,...) \
	std::cout << YELLOW_t <<"debug:["<< __FILE__ << ":"<< __LINE__ << "]:"<< Sutil::Format(msg, ##__VA_ARGS__) <<BASECOLOER<< std::endl;
#define infoS(msg,...) \
    std::cout << GREEN_t <<"debug:["<< __FILE__ << ":" << __LINE__ << "]:"<<  Sutil::Format(msg, ##__VA_ARGS__)<< BASECOLOER <<std::endl;

inline std::string change(char c)
{
     std::string data;
    for(int i=0;i<8;i++)
    {
        //  data+=c&(0x01<<i);
        if ( (( c >>(i-1) ) & 0x01) == 1 )
        {
            data+="1";
        }
        else
        {
            data+="0";
        }
    }
    for(int a=1;a<5;a++)
    {
        char x=data[a];
        data[a]=data[8-a];
        data[8-a]=x;
    }
   return data;
}

inline std::string ToBin(const char * bt,size_t len){
    std::string ret;
    for(int i=0;i< len;i++){
        ret+=change(bt[i]);
    }
    return ret;
}




#endif // !__DEBUG__L


