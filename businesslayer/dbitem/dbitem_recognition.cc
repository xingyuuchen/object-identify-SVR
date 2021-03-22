#include "dbitem_recognition.h"


const char * const DBItem_Recognition::table_ = "recognitions";
const char * const DBItem_Recognition::field_name_recog_id_ = "recog_id";
const char * const DBItem_Recognition::field_name_item_name_ = "item_name";
const char * const DBItem_Recognition::field_name_item_type_ = "item_type";
const char * const DBItem_Recognition::field_name_item_desc_ = "item_desc";


DBItem_Recognition::DBItem_Recognition()
        : recog_id_(0)
        , item_type_(0)
        , has_recog_id_(false)
        , has_item_name_(false)
        , has_item_desc_(false)
        , has_item_type_(false) { }

const char *DBItem_Recognition::table() const { return table_; }

uint32_t DBItem_Recognition::GetRecogId() const { return recog_id_; }

std::string &DBItem_Recognition::GetItemName() { return item_name_; }

std::string &DBItem_Recognition::GetItemDesc() { return item_desc_; }

int DBItem_Recognition::GetItemType() const { return item_type_; }

void DBItem_Recognition::PopulateFieldList(std::vector<std::string> &_field_list) const {
    if (has_recog_id_) {
        _field_list.emplace_back(field_name_recog_id_);
    }
    if (has_item_name_) {
        _field_list.emplace_back(field_name_item_name_);
    }
    if (has_item_type_) {
        _field_list.emplace_back(field_name_item_type_);
    }
    if (has_item_desc_) {
        _field_list.emplace_back(field_name_item_desc_);
    }
}

void DBItem_Recognition::PopulateValueList(std::vector<std::string> &_value_list) const {
    if (has_recog_id_) {
        _value_list.emplace_back(std::to_string(recog_id_));
    }
    if (has_item_name_) {
        _value_list.emplace_back(std::string("'") + item_name_ + std::string("'"));
    }
    if (has_item_type_) {
        _value_list.emplace_back(std::to_string(item_type_));
    }
    if (has_item_desc_) {
        _value_list.emplace_back(std::string("'") + item_desc_ + std::string("'"));
    }
}

void DBItem_Recognition::PopulateEqualList(std::map<std::string, std::string> &_equal_list) const {
    if (has_recog_id_) {
        _equal_list.insert(std::make_pair(field_name_recog_id_, std::to_string(recog_id_)));
    }
    if (has_item_name_) {
        _equal_list.insert(std::make_pair(field_name_item_name_,
                         std::string("'") + item_name_ + std::string("'")));
    }
    if (has_item_type_) {
        _equal_list.insert(std::make_pair(field_name_item_type_,
                          std::to_string(item_type_)));
    }
    if (has_item_desc_) {
        _equal_list.insert(std::make_pair(field_name_item_desc_,
                          std::string("'") + item_desc_ + std::string("'")));
    }
}

void DBItem_Recognition::SetRecogId(uint32_t _recog_id_) {
    recog_id_ = _recog_id_;
    has_recog_id_ = true;
}

void DBItem_Recognition::SetItemName(std::string _item_name) {
    item_name_ = std::move(_item_name);
    has_item_name_ = true;
}

void DBItem_Recognition::SetItemDesc(std::string _item_desc) {
    item_desc_ = std::move(_item_desc);
    has_item_desc_ = true;
}

void DBItem_Recognition::SetItemType(int _item_type) {
    item_type_ = _item_type;
    has_item_type_ = true;
}
