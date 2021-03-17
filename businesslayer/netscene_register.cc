#include "netscene_register.h"
#include "constantsprotocol.h"
#include "log.h"
#include "netsceneregister.pb.h"


NetSceneRegister::NetSceneRegister() : NetSceneBase() {

}

int NetSceneRegister::GetType() { return kNetSceneTypeRegister; }

NetSceneBase *NetSceneRegister::NewInstance() { return new NetSceneRegister(); }


int NetSceneRegister::DoSceneImpl(const std::string &_in_buffer) {
    static std::mutex   mutex_;
    static uint32_t usr_id = 0;
    uint32_t id;
    {
        std::unique_lock<std::mutex> lock(mutex_);
        id = ++usr_id;
    }
    LogI(__FILE__, "[DoSceneImpl] alloc id: %d", id)
    NetSceneRegisterProto::NetSceneRegisterResp resp;
    
    resp.set_usr_id(id);
    
    size_t size = resp.ByteSizeLong();
    std::string byte_string;
    resp.SerializeToString(&byte_string);
    CopyRespToSendBody(byte_string, size);
    return 0;
}

