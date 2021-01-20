#include "httprequest.h"
#include "requestline.h"
#include "headerfield.h"


namespace http { namespace request {


void Pack(const std::string &_host, const std::string &_url, const std::map<std::string, std::string> _headers,
          AutoBuffer& _send_body, AutoBuffer &_out_buff) {
    _out_buff.Reset();
    
    RequestLine request_line;
    request_line.SetMethod(http::RequestLine::kPOST);
    request_line.SetVersion(http::RequestLine::kHTTP_1_1);
    request_line.SetUrl(_url);
    request_line.AppendToBuffer(_out_buff);
    
    HeaderField header_field;
    for (auto iter = _headers.begin(); iter != _headers.end(); iter++) {
        header_field.InsertOrUpdateHeader(iter->first, iter->second);
    }
    header_field.InsertOrUpdateHeader(HeaderField::KHost, _host);
    header_field.InsertOrUpdateHeader(HeaderField::KConnection, HeaderField::KConnectionClose);
    
//    header_field.InsertOrUpdateHeader(HeaderField::KContentLength, _send_body.Length());
    header_field.AppendToBuffer(_out_buff);
}



}}

