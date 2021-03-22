#include "dbitem_userinfo.h"

const char * const DBItem_UserInfo::table_ = "users";
const char * const DBItem_UserInfo::field_name_usrid_ = "usrid";
const char * const DBItem_UserInfo::field_name_nickname_ = "nickname";
const char * const DBItem_UserInfo::field_name_avatarpath_ = "avatar_path";

DBItem_UserInfo::DBItem_UserInfo()
        : has_usr_id_(false)
        , has_nickname_(false)
        , has_avatar_path_(false)
        , usr_id_(0) {
}

const char *DBItem_UserInfo::table() const { return table_; }

uint32_t DBItem_UserInfo::GetUsrId() const { return usr_id_; }

std::string &DBItem_UserInfo::GetNickname() { return nickname_; }

std::string &DBItem_UserInfo::GetAvatarPath() { return avatar_path_; }


void DBItem_UserInfo::SetUsrId(uint32_t _usr_id) {
    usr_id_ = _usr_id;
    has_usr_id_ = true;
}

void DBItem_UserInfo::SetNickname(std::string _nickname) {
    nickname_ = std::move(_nickname);
    has_nickname_ = true;
}

void DBItem_UserInfo::SetAvatarPath(std::string _avatar_path) {
    avatar_path_ = std::move(_avatar_path);
    has_avatar_path_ = true;
}

void DBItem_UserInfo::PopulateFieldList(std::vector<std::string> &_field_list) const {
    if (has_usr_id_) {
        _field_list.emplace_back(field_name_usrid_);
    }
    if (has_nickname_) {
        _field_list.emplace_back(field_name_nickname_);
    }
    if (has_avatar_path_) {
        _field_list.emplace_back(field_name_avatarpath_);
    }
}

void DBItem_UserInfo::PopulateValueList(std::vector<std::string> &_value_list) const {
    if (has_usr_id_) {
        _value_list.push_back(std::to_string(usr_id_));
    }
    if (has_nickname_) {
        _value_list.push_back(std::string("'") + nickname_ + std::string("'"));
    }
    if (has_avatar_path_) {
        _value_list.push_back(std::string("'") + avatar_path_ + std::string("'"));
    }
}

void DBItem_UserInfo::PopulateEqualList(std::map<std::string, std::string> &_equal_list) const {
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

