#include "dbitem_bearuser.h"

const char * const DBItem_BearUser::table_ = "bear_users";
const char * const DBItem_BearUser::field_name_usrid_ = "usrid";
const char * const DBItem_BearUser::field_name_usr_name_ = "usrname";
const char * const DBItem_BearUser::field_name_pwd_ = "pwd";


DBItem_BearUser::DBItem_BearUser()
        : has_usr_id_(false)
        , has_usr_name_(false)
        , has_usr_pwd_(false)
        , usr_id_(0) {
}

const char *DBItem_BearUser::table() const { return table_; }


void DBItem_BearUser::PopulateFieldList(std::vector<std::string> &_field_list) const {
    if (has_usr_id_) {
        _field_list.emplace_back(field_name_usrid_);
    }
    if (has_usr_name_) {
        _field_list.emplace_back(field_name_usr_name_);
    }
    if (has_usr_pwd_) {
        _field_list.emplace_back(field_name_pwd_);
    }
}

void DBItem_BearUser::PopulateValueList(std::vector<std::string> &_value_list) const {
    if (has_usr_id_) {
        _value_list.push_back(std::to_string(usr_id_));
    }
    if (has_usr_name_) {
        _value_list.emplace_back(std::string("'") + usr_name_ + std::string("'"));
    }
    if (has_usr_pwd_) {
        _value_list.emplace_back(std::string("'") + usr_pwd_ + std::string("'"));
    }
}

void DBItem_BearUser::PopulateEqualList(std::map<std::string, std::string> &_equal_list) const {
    if (has_usr_id_) {
        _equal_list.insert(std::make_pair(field_name_usrid_, std::to_string(usr_id_)));
    }
    if (has_usr_name_) {
        _equal_list.insert(std::make_pair(field_name_usr_name_,
                  std::string("'") + usr_name_ + std::string("'")));
    }
    if (has_usr_pwd_) {
        _equal_list.insert(std::make_pair(field_name_pwd_,
                  std::string("'") + usr_pwd_ + std::string("'")));
    }
}

void DBItem_BearUser::SetUserName(std::string _usr_name) {
    usr_name_ = std::move(_usr_name);
    has_usr_name_ = true;
}

void DBItem_BearUser::SetUserPwd(std::string _usr_pwd) {
    usr_pwd_ = std::move(_usr_pwd);
    has_usr_pwd_ = true;
}

void DBItem_BearUser::SetUserId(uint32_t _usr_id) {
    usr_id_ = _usr_id;
    has_usr_id_ = true;
}

uint32_t DBItem_BearUser::GetUsrId() const { return usr_id_; }

std::string &DBItem_BearUser::GetUsrName() { return usr_name_; }

std::string &DBItem_BearUser::GetUsrPwd() { return usr_pwd_; }
