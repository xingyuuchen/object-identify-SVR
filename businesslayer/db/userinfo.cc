#include "userinfo.h"

const char *UserInfo::table_ = "users";
const char *UserInfo::field_name_usrid_ = "usrid";
const char *UserInfo::field_name_nickname_ = "nickname";
const char *UserInfo::field_name_avatarpath_ = "avatar_path";

UserInfo::UserInfo()
        : has_usr_id_(false)
        , has_nickname_(false)
        , has_avatar_path_(false)
        , usr_id_(0) {
}

const char *UserInfo::table() const { return table_; }

uint32_t UserInfo::GetUsrId() const { return usr_id_; }

std::string &UserInfo::GetNickname() { return nickname_; }

std::string &UserInfo::GetAvatarPath() { return avatar_path_; }


void UserInfo::SetUsrId(uint32_t _usr_id) {
    usr_id_ = _usr_id;
    has_usr_id_ = true;
}

void UserInfo::SetNickname(std::string _nickname) {
    nickname_ = std::move(_nickname);
    has_nickname_ = true;
}

void UserInfo::SetAvatarPath(std::string _avatar_path) {
    avatar_path_ = std::move(_avatar_path);
    has_avatar_path_ = true;
}

void UserInfo::PopulateFieldList(std::vector<std::string> &_filed_list) const {
    if (has_usr_id_) {
        _filed_list.emplace_back(field_name_usrid_);
    }
    if (has_nickname_) {
        _filed_list.emplace_back(std::string("'") + field_name_nickname_ + std::string("'"));
    }
    if (has_avatar_path_) {
        _filed_list.emplace_back(std::string("'") + field_name_avatarpath_ + std::string("'"));
    }
}

void UserInfo::PopulateValueList(std::vector<std::string> &_value_list) const {
    if (has_usr_id_) {
        _value_list.push_back(std::to_string(usr_id_));
    }
    if (has_nickname_) {
        _value_list.push_back(nickname_);
    }
    if (has_avatar_path_) {
        _value_list.push_back(avatar_path_);
    }
}

void UserInfo::PopulateEqualList(std::map<std::string, std::string> &_equal_list) const {
    if (has_usr_id_) {
        _equal_list.insert(std::make_pair(field_name_usrid_, std::to_string(usr_id_)));
    }
    if (has_nickname_) {
        _equal_list.insert(std::make_pair(field_name_nickname_,
                  std::string("'") + nickname_ + std::string("'")));
    }
    if (has_avatar_path_) {
        _equal_list.insert(std::make_pair(field_name_avatarpath_,
                  std::string("'") + avatar_path_ + std::string("'")));
    }
}

