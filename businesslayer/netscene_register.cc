#include "netscene_register.h"
#include <utility>
#include "netscenetypes.h"
#include "log.h"
#include "dao/connection.h"
#include "constantsprotocol.h"


NetSceneRegister::NetSceneRegister() : NetSceneProtobuf() {
}

int NetSceneRegister::GetType() { return kNetSceneTypeRegister; }

NetSceneBase *NetSceneRegister::NewInstance() { return new NetSceneRegister(); }

NetSceneProtobuf::RespMessage *NetSceneRegister::GetRespMessage() { return &resp_; }

int NetSceneRegister::DoSceneImpl(const std::string &_in_buffer) {
    static std::mutex mutex_;
    int32_t id = -1;
    
    DBItem_UserInfo new_usr;
    new_usr.SetNickname("NULL");
    new_usr.SetAvatarPath("NULL");
    int ret = Dao::Insert(new_usr);
    
    if (ret > 0) {
        id = ret;
    } else {
        errcode_ = kErrDatabase;
        errmsg_ = "insert database failed";
    }
    
    LogI("alloc id: %d", id)
    
    resp_.set_usr_id(id);
    
    return 0;
}
