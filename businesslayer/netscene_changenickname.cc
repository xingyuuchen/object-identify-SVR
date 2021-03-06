#include "netscene_changenickname.h"
#include "log.h"
#include "netscenetypes.h"
#include <string>
#include "constantsprotocol.h"
#include "dao/connection.h"


NetSceneChangeNickname::NetSceneChangeNickname()
        : NetSceneProtobuf() {
}

int NetSceneChangeNickname::GetType() { return kNetSceneTypeChangeNickname; }

NetSceneBase *NetSceneChangeNickname::NewInstance() { return new NetSceneChangeNickname(); }

NetSceneProtobuf::RespMessage *NetSceneChangeNickname::GetRespMessage() { return &resp_; }

int NetSceneChangeNickname::DoSceneImpl(const std::string &_in_buffer) {
    LogI("req.len: %zd", _in_buffer.size());
    
    NetSceneChangeNicknameProto::NetSceneChangeNicknameReq req;
    req.ParseFromArray(_in_buffer.data(), _in_buffer.size());
    uint32_t usr_id = req.usr_id();
    std::string nickname = req.nickname();
    
    DBItem_UserInfo old;
    old.SetUsrId(usr_id);
    DBItem_UserInfo neo;
    neo.SetNickname(nickname);
    
    int db_ret = dao::Update(old, neo);
    if (db_ret < 0) {
        LogI("db err, usrid: %d, nickname: %s",
             usr_id, nickname.c_str())
        errcode_ = kErrDatabase;
        errmsg_ = "db err.";
    }
    LogI("update succeed, usrid: %d, nickname: %s",
         usr_id, nickname.c_str())
    resp_.set_nop(true);
    
    return 0;
}
