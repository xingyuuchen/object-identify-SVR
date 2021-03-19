#include "netscene_register.h"
#include <utility>
#include "netscenetypes.h"
#include "log.h"
#include "netsceneregister.pb.h"
#include "dao/connection.h"
#include "constantsprotocol.h"


NetSceneRegister::NetSceneRegister() : NetSceneBase() {
}

int NetSceneRegister::GetType() { return kNetSceneTypeRegister; }

NetSceneBase *NetSceneRegister::NewInstance() { return new NetSceneRegister(); }


int NetSceneRegister::DoSceneImpl(const std::string &_in_buffer) {
    static std::mutex mutex_;
    int32_t id = -1;
    
    UserInfo new_usr;
    new_usr.SetNickname("NULL");
    new_usr.SetAvatarPath("NULL");
    int ret = Dao::Insert(new_usr);
    
    if (ret > 0) {
        id = ret;
    } else {
        base_resp_.set_errcode(kErrDatabase);
        base_resp_.set_errmsg("insert database failed");
    }
    
    LogI(__FILE__, "[DoSceneImpl] alloc id: %d", id)
    NetSceneRegisterProto::NetSceneRegisterResp resp;
    
    resp.set_usr_id(id);
    
    size_t size = resp.ByteSizeLong();
    std::string byte_string;
    resp.SerializeToString(&byte_string);
    Write2BaseResp(byte_string, size);
    return 0;
}
