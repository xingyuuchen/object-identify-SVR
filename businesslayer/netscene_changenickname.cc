#include "netscene_changenickname.h"
#include "log.h"
#include "netscenetypes.h"
#include "netscenechangenickname.pb.h"
#include <string>
#include "constantsprotocol.h"
#include "dao/connection.h"


NetSceneChangeNickname::NetSceneChangeNickname()
        : NetSceneBase() {
}

int NetSceneChangeNickname::GetType() { return kNetSceneTypeChangeNickname; }

NetSceneBase *NetSceneChangeNickname::NewInstance() { return new NetSceneChangeNickname(); }

int NetSceneChangeNickname::DoSceneImpl(const std::string &_in_buffer) {
    LogI(__FILE__, "[DoSceneImpl] req.len: %zd", _in_buffer.size());
    if (socket_ <= 0) {
        LogE(__FILE__, "[NetSceneQueryImg] Socket NOT open");
        return -1;
    }
    NetSceneChangeNicknameProto::NetSceneChangeNicknameReq req;
    req.ParseFromArray(_in_buffer.data(), _in_buffer.size());
    uint32_t usr_id = req.usr_id();
    std::string nickname = req.nickname();
    
    UserInfo old;
    old.SetUsrId(usr_id);
    UserInfo neo;
    neo.SetNickname(nickname);
    
    int db_ret = Dao::Update(old, neo);
    if (db_ret < 0) {
        LogI(__FILE__, "[DoSceneImpl] db err, usrid: %d, nickname: %s",
             usr_id, nickname.c_str())
        base_resp_.set_errcode(kErrDatabase);
        base_resp_.set_errmsg("db err.");
    }
    NetSceneChangeNicknameProto::NetSceneChangeNicknameResp resp;
    resp.set_nop(true);
    
    size_t size = resp.ByteSizeLong();
    std::string byte_string;
    resp.SerializeToString(&byte_string);
    Write2BaseResp(byte_string, size);
    return 0;
}
