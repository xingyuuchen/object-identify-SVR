#pragma once
#include "dbitem.h"


class DBItem_Recognition : public DBItem {
  public:
    
    DBItem_Recognition();
    
    const char *table() const final;
    
    uint32_t GetRecogId() const;
    
    std::string &GetItemName();
    
    std::string &GetItemDesc();
    
    int GetItemType() const;
    
    void SetRecogId(uint32_t _recog_id_);
    
    void SetItemName(std::string _item_name);
    
    void SetItemDesc(std::string _item_desc);
    
    void SetItemType(int _item_type);
    
    void PopulateFieldList(std::vector<std::string> &_field_list) const override;
    
    void PopulateValueList(std::vector<std::string> &_value_list) const override;
    
    void PopulateEqualList(std::map<std::string, std::string> &_equal_list) const override;

  public:
    static const char * const table_;
    static const char * const field_name_recog_id_;
    static const char * const field_name_item_name_;
    static const char * const field_name_item_type_;
    static const char * const field_name_item_desc_;

  private:
    uint32_t            recog_id_;
    std::string         item_name_;
    std::string         item_desc_;
    int                 item_type_;
    bool                has_recog_id_;
    bool                has_item_name_;
    bool                has_item_desc_;
    bool                has_item_type_;
};

