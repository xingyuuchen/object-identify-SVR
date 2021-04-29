#pragma once
#include "dbitem.h"


class DBItem_VirusUser : public DBItem {
  public:
    
    DBItem_VirusUser();
    
    const char *table() const final;
    
    void SetUserId(uint32_t _usr_id);
    
    void SetEmail(std::string _email);
    
    void SetUserPwd(std::string _password);
    
    void SetUserName(std::string _username);
    
    uint32_t GetUsrId() const;
    
    std::string &GetEmail();
    
    std::string &GetUsrPwd();
    
    std::string &GetUsrName();
    
    void PopulateFieldList(std::vector<std::string> &_field_list) const override;
    
    void PopulateValueList(std::vector<std::string> &_value_list) const override;
    
    void PopulateEqualList(std::map<std::string, std::string> &_equal_list) const override;

  public:
    static const char * const table_;
    static const char * const field_name_usrid_;
    static const char * const field_name_email_;
    static const char * const field_name_password_;
    static const char * const field_name_username_;

  private:
    uint32_t            usr_id_;
    std::string         email_;
    std::string         password_;
    std::string         username_;
    bool                has_usr_id_;
    bool                has_email_;
    bool                has_password_;
    bool                has_username_;
};
