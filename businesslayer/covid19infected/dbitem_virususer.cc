#include "dbitem_virususer.h"

const char * const DBItem_VirusUser::table_ = "virus_users";
const char * const DBItem_VirusUser::field_name_usrid_ = "usrid";
const char * const DBItem_VirusUser::field_name_email_ = "email";
const char * const DBItem_VirusUser::field_name_password_ = "pwd";
const char * const DBItem_VirusUser::field_name_username_ = "username";


DBItem_VirusUser::DBItem_VirusUser()
        : has_email_(false)
        , has_password_(false)
        , has_username_(false)
        , has_usr_id_(false)
        , usr_id_(0) {
}

const char *DBItem_VirusUser::table() const { return table_; }

void DBItem_VirusUser::PopulateFieldList(std::vector<std::string> &_field_list) const {
    if (has_usr_id_) {
        _field_list.emplace_back(field_name_usrid_);
    }
    if (has_email_) {
        _field_list.emplace_back(field_name_email_);
    }
    if (has_password_) {
        _field_list.emplace_back(field_name_password_);
    }
    if (has_username_) {
        _field_list.emplace_back(field_name_username_);
    }
}

void DBItem_VirusUser::PopulateValueList(std::vector<std::string> &_value_list) const {
    if (has_usr_id_) {
        _value_list.push_back(std::to_string(usr_id_));
    }
    if (has_email_) {
        _value_list.emplace_back(std::string("'") + email_ + std::string("'"));
    }
    if (has_username_) {
        _value_list.emplace_back(std::string("'") + username_ + std::string("'"));
    }
    if (has_password_) {
        _value_list.emplace_back(std::string("'") + password_ + std::string("'"));
    }
}

void DBItem_VirusUser::PopulateEqualList(std::map<std::string, std::string> &_equal_list) const {
    if (has_usr_id_) {
        _equal_list.insert(std::make_pair(field_name_usrid_, std::to_string(usr_id_)));
    }
    if (has_email_) {
        _equal_list.insert(std::make_pair(field_name_email_,
                  std::string("'") + email_ + std::string("'")));
    }
    if (has_username_) {
        _equal_list.insert(std::make_pair(field_name_username_,
                  std::string("'") + username_ + std::string("'")));
    }
    if (has_password_) {
        _equal_list.insert(std::make_pair(field_name_password_,
                  std::string("'") + password_ + std::string("'")));
    }
}

void DBItem_VirusUser::SetUserId(uint32_t _usr_id) {
    usr_id_ = _usr_id;
    has_usr_id_ = true;
}

void DBItem_VirusUser::SetEmail(std::string _email) {
    email_ = std::move(_email);
    has_email_ = true;
}

void DBItem_VirusUser::SetUserPwd(std::string _password) {
    password_ = std::move(_password);
    has_password_ = true;
}

void DBItem_VirusUser::SetUserName(std::string _username) {
    username_ = std::move(_username);
    has_username_ = true;
}

uint32_t DBItem_VirusUser::GetUsrId() const { return usr_id_; }

std::string &DBItem_VirusUser::GetEmail() { return email_; }

std::string &DBItem_VirusUser::GetUsrPwd() { return password_; }

std::string &DBItem_VirusUser::GetUsrName() { return username_; }
