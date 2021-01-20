#include "requestline.h"

namespace http {


RequestLine::RequestLine()
    : method_(kPOST)
    , version_(kHTTP_1_1)
    , url_() {}
    
    
const char *const RequestLine::method2string[] = {
        "GET",
        "POST",
        "DELETE"
};

const char *const RequestLine::version2string[] = {
        "HTTP/0.9",
        "HTTP/1.0",
        "HTTP/1.1",
        "HTTP/2.0",
};
    
void RequestLine::SetMethod(THttpMethod _method) { method_ = _method; }

void RequestLine::SetUrl(const std::string &_url) { url_ = _url; }

void RequestLine::SetVersion(THttpVersion _version) { version_ = _version; }

void RequestLine::ToString(std::string &_target) {
    _target.clear();
    _target += method2string[method_];
    _target += " ";
    _target += url_;
    _target += " ";
    _target += version2string[version_];
    _target += "\r\n";
}

void RequestLine::AppendToBuffer(AutoBuffer &_buffer) {
    std::string str;
    ToString(str);
    _buffer.Write((unsigned char *) str.data(), str.size());
}


}