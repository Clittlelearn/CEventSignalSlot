#include "argpaser.h"
#include "debug.h"
#include <algorithm>
#include <string>
#include <vector>
#include <sstream>
#include <regex>
LOGINSTANCE("paser.log")
std::vector<std::string> Stringsplit(const std::string& str, const std::string& split)
{
    
    std::regex reg(split);
    std::vector<std::string> elems(std::sregex_token_iterator(str.begin(), str.end(), reg, -1),
                                   std::sregex_token_iterator());
    return elems;
}




PaserReslut::PaserReslut(){}
PaserReslut::PaserReslut(const PaserReslut && res){
    OptionValue=std::move(res.OptionValue);
    CmdArg=std::move(res.CmdArg);
    CmdName=std::move(res.CmdName);
    b_isOk=res.b_isOk;
}
PaserReslut & PaserReslut::operator=(const PaserReslut && otherRes){
    OptionValue=std::move(otherRes.OptionValue);
    CmdArg=std::move(otherRes.CmdArg);
    CmdName=std::move(otherRes.CmdName);
    b_isOk=otherRes.b_isOk;
    return *this;
}
std::vector<std::string>  PaserReslut::GetOptionValue(const std::string & option,bool & exits){
    auto iter=OptionValue.find(option);
    if(iter!=OptionValue.end()){
        exits=true;
        return OptionValue[option];
    }
    exits=false;
    return {};
}
std::vector<std::string> & PaserReslut::GetInputArg(){
    return CmdArg;
}



void Commd::AddOption(const std::string &Option_str,bool hasValue,const std::string & description){
    Options[Option_str]=OptionDes{hasValue,description};
}

bool PaserReslut::isOk(){
    return b_isOk;
}

Commd::Commd(const std::string & cmd)
{
    m_cmd=cmd;
}
PaserReslut Commd::Parse(const std::vector<std::string> & cmd){
    PaserReslut res;
    res.b_isOk=false;
    enum css_{
        COMMD_,
        OPTION_,
        ARG_
    };
    auto _c_type_check=[&](const std::string & v) ->css_
    {
        if(v==m_cmd){
            return COMMD_;
        }
        OptionDes opd;
        if(isOption(v, opd)){
            return OPTION_;
        }
        return ARG_;
    };
   //auto ptd= Stringsplit(cmd,"\\s+");
    for(int i=0;i<cmd.size();i++){
        css_ st=_c_type_check(cmd[i]);
        
        switch (st) {
            case COMMD_:
                break;
            case OPTION_:
                i=PaseOption(cmd[i],cmd,res, i+1);
                break;
            case ARG_:
                res.CmdArg.push_back(cmd[i]);
                break;
            default:
                errorL("error type");
                break;
        }
    }
    
    res.CmdName=m_cmd;
    res.b_isOk=true;
    return res;


}

std::string Commd::GetName()
{
    return m_cmd;
}

bool Commd::isOption(const std::string & cmd,OptionDes& des){

    auto iter=Options.find(cmd);
    if (iter!=Options.end()){
        return true;
    }
    return false;
}

int Commd::PaseOption(const std::string & opt,const std::vector<std::string> & vec,PaserReslut & res,int index)
{
    OptionDes des;
    int i=index;
    for(;i<vec.size();i++){
        if(isOption(vec[i], des)|| vec[i]==m_cmd){
            return i;
        }
        res.OptionValue[opt].push_back(vec[i]);
    }
    return i;
}





void ArgsPaser::addCommd(Commd::ptr comd)
{
   std::string cmd=comd->GetName();
     auto iter=allCommds.find(cmd);
    if (iter!=allCommds.end()){
        debugL("commd exits!!");
        return;
    }else{
        allCommds[cmd]=comd;
    }
}



PaserReslut ArgsPaser::Paser(const std::string & input)
{
    PaserReslut res;
    std::vector<std::string> ptd= Stringsplit(input,"\\s+");
    if(ptd.empty()){
        errorL("pase fail input pase empty");
        return res;
    }
    Commd::ptr nP;
    if(!isCommd(ptd[0],nP)){
        errorL("not found commd %s",ptd[0]);
        return res;
    }   
    res=  nP->Parse(ptd);
    return res;


}

bool ArgsPaser::isCommd(const std::string& commd,Commd::ptr & p)
{
    auto iter=allCommds.find(commd);
    if(iter!=allCommds.end()){
        p=iter->second;
        return true;
    }
    return false;
}

