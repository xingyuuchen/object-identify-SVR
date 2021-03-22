#pragma once
#include "dbitem.h"


class DBItem_UserInfo : public DBItem {
  public:
    
    DBItem_UserInfo();
    
    const char *table() const final;
    
    uint32_t GetUsrId() const;
    
    std::string &GetNickname();
    
    std::string &GetAvatarPath();
    
    void SetUsrId(uint32_t _usr_id);
    
    void SetNickname(std::string _nickname);
    
    void SetAvatarPath(std::string _avatar_path);
    
    void PopulateFieldList(std::vector<std::string> &_field_list) const override;
    
    void PopulateValueList(std::vector<std::string> &_value_list) const override;
    
    void PopulateEqualList(std::map<std::string, std::string> &_equal_list) const override;

  public:
    static const char * const table_;
    static const char * const field_name_usrid_;
    static const char * const field_name_nickname_;
    static const char * const field_name_avatarpath_;
    
  private:
    uint32_t            usr_id_;
    std::string         nickname_;
    std::string         avatar_path_;
    bool                has_usr_id_;
    bool                has_nickname_;
    bool                has_avatar_path_;
};
