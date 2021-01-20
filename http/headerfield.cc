#include "headerfield.h"


namespace http {

const char *const HeaderField::KHost = "Host";
const char *const HeaderField::KContentLength = "Content-Length";
const char *const HeaderField::KTransferEncoding = "Content-Length";
const char *const HeaderField::KConnection = "Connection";


const char *const HeaderField::KOctetStream = "application/octet-stream";
const char *const HeaderField::KConnectionClose = "close";


void HeaderField::InsertOrUpdateHeader(const std::string &_key,
                                       const std::string &_value) {
    header_fields_[_key] = _value;
}


size_t HeaderField::GetHeaderSize() {
    size_t ret = 0;
    for (auto entry = header_fields_.begin(); entry != header_fields_.end(); entry++) {
        ret += entry->first.size();
        ret += entry->second.size();
        ret += 3;
    }
    return ret;
}


void HeaderField::ToString(std::string &_target) {
    _target.clear();
    for (auto entry = header_fields_.begin(); entry != header_fields_.end(); entry++) {
        _target += entry->first;
        _target += ":";
        _target += entry->second;
        _target += "\r\n";
    }
}


void HeaderField::AppendToBuffer(AutoBuffer &_out_buff) {
    std::string str;
    ToString(str);
    _out_buff.Write((unsigned char *) str.data(), str.size());
}


}
