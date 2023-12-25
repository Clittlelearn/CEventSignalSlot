#ifndef _ARG_PASE_
#define _ARG_PASE_
#include <memory>
#include <string>
#include <map>
#include <vector>
struct OptionDes{
    bool hasValue;
    std::string description;
};


template <typename T>
class create_base{
    public:
      template<typename ...Ts>
     static  std::shared_ptr<T> create(const Ts&... ts){
            T * ob=new T(ts...);
            return std::shared_ptr<T>(ob);
      }
      
};


class Commd;
class PaserReslut{
    public:
    PaserReslut();
    PaserReslut(const PaserReslut & res )=delete;
    PaserReslut & operator=(const PaserReslut & otherRes)=delete;
    PaserReslut(const PaserReslut && res);
    PaserReslut & operator=(const PaserReslut && otherRes);
    std::vector<std::string>  GetOptionValue(const std::string & option,bool & exits);
    std::vector<std::string> & GetInputArg();
    std::string GetCommdName(){
        return CmdName;
    }
    bool isOk();
    private:
    friend Commd;
    std::map<std::string,std::vector<std::string>> OptionValue;
    std::vector<std::string> CmdArg;
    std::string CmdName;
    bool b_isOk=false;
};

class Commd:public create_base<Commd>{
    public:
    using ptr=std::shared_ptr<Commd>;
    Commd(const std::string & cmd);
    void AddOption(const std::string &Option,bool hasValue,const std::string & description);
    PaserReslut Parse(const std::vector<std::string> & lineStr);
    std::string GetName();
private:
    bool isOption(const std::string & str,OptionDes & des);
    int PaseOption(const std::string & opt,const std::vector<std::string> & vec,PaserReslut & res,int index);
    std::string m_cmd;
    std::map<std::string,OptionDes> Options;
};

class ArgsPaser:public create_base<ArgsPaser>{
public:
    ArgsPaser()=default;
    using ptr=std::shared_ptr<ArgsPaser>;
    void addCommd(Commd::ptr comd);
    PaserReslut Paser(const std::string & input);
    bool isCommd(const std::string & commd,Commd::ptr & p);
    private:

    std::map<std::string,Commd::ptr> allCommds;
};

#endif