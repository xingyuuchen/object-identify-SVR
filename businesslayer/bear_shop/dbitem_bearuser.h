#pragma once
#include "dbitem.h"


class DBItem_BearUser : public DBItem {
  public:
    
    DBItem_BearUser();
    
    const char *table() const final;
    
    void SetUserName(std::string _usr_name);
    
    void SetUserPwd(std::string _usr_pwd);
    
    void SetUserId(uint32_t _usr_id);
    
    uint32_t GetUsrId() const;
    
    std::string &GetUsrName();
    
    std::string &GetUsrPwd();
    
    void PopulateFieldList(std::vector<std::string> &_field_list) const override;
    
    void PopulateValueList(std::vector<std::string> &_value_list) const override;
    
    void PopulateEqualList(std::map<std::string, std::string> &_equal_list) const override;

  public:
    static const char * const table_;
    static const char * const field_name_usrid_;
    static const char * const field_name_usr_name_;
    static const char * const field_name_pwd_;

  private:
    uint32_t            usr_id_;
    std::string         usr_name_;
    std::string         usr_pwd_;
    bool                has_usr_id_;
    bool                has_usr_name_;
    bool                has_usr_pwd_;
};
