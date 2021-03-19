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
    
    RowUserInfo new_usr("'NULL'", "'NULL'");
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


const char *RowUserInfo::table_ = "users";
const char *RowUserInfo::field_name1_ = "nickname";
const char *RowUserInfo::field_name2_ = "avatar_path";

uint64_t RowUserInfo::GetUsrId() const { return usr_id_; }

RowUserInfo::RowUserInfo(std::string _nickname, std::string _avatar_path)
        : nickname_(std::move(_nickname))
        , avatar_path_(std::move(_avatar_path))
        , usr_id_(0) {
    OnDataPrepared();
}

const char *RowUserInfo::table() const {
    return table_;
}

void RowUserInfo::PopulateFieldList(std::vector<std::string> &_filed_list) const {
    _filed_list.emplace_back(field_name1_);
    _filed_list.emplace_back(field_name2_);
}

void RowUserInfo::PopulateValueList(std::vector<std::string> &_value_list) const {
    _value_list.push_back(nickname_);
    _value_list.push_back(avatar_path_);
}
