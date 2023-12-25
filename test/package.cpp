
#include "package.h"
#include "debug.h"
#include <cstdint>
LOGINSTANCE("BufferToproto.log")

ReadBox BufferToproto::Serialize(const std::string &pdata,const std::string & protoName) {
    proto_package pkage;
    pkage.set_name(protoName);
    pkage.set_data(pdata);
    std::string data=pkage.SerializeAsString();

    unsigned char *buf = new unsigned char[data.size() + sizeof(uint64_t)];
    uint64_t lenght;
    lenght = data.size();
    memcpy(buf, (void *)&lenght, sizeof(uint64_t));
    unsigned char *p = buf + sizeof(uint64_t);
    memcpy(p, data.c_str(), data.length());
    ReadBox ret(buf, data.size() + sizeof(uint64_t),true);
    return ret;
}

BufferToproto::BUFFERSTAT BufferToproto::GetPackage(proto_package &pk) {
    if(lenght==0){
        if(queue.lenght() < sizeof(uint64_t)){
            debugL("queue.lenght:%s",queue.lenght());
            return NOTCOM;
        }
        queue.read((unsigned char *)&lenght, sizeof(int64_t));
    }
    if (queue.lenght() < lenght) {
        debugL("queue.lenght:%s ,lenght:%s",queue.lenght(),lenght);
       return NOTCOM;
    }
    std::string dat=queue.read(lenght);
    lenght=0;
    bool fg=pk.ParseFromString(std::move(dat));
    if (!fg) {
        errorL("pase fail!");
        return PASEFAIL;
    }
    return OK;
}
void BufferToproto::Push(const ReadBox &box) {
    queue.push(box);
}