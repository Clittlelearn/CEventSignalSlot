#ifndef _FILE_SERVER_H

#define _FILE_SERVER_H

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <functional>
#include <memory>
#include <mutex>
#include <ostream>
#include <queue>
#include <string>
#include <string_view>
#include <sys/types.h>
#include <vector>
#include "BLLinuxTCPSocket.h"
#include "BLObject.h"
#include "debug.h"
#include "linux/BLP2P.h"
#include "Singleton.h"
#include "linux/BLLinuxTCPSocket.h"
#include "utils/Box.hpp"
#include "datacode.pb.h"

class BufferToproto{
public:
    enum BUFFERSTAT{
        OK,
        NOTCOM,
        PASEFAIL
    };

    static ReadBox Serialize(const std::string & data,const std::string & protoName);

    BUFFERSTAT GetPackage(proto_package & pk);

    void Push(const ReadBox & box);
private:
    uint64_t lenght=0;
    bool packaged=false;
    ReadBoxQueue queue;
    
};



#endif