#include "netscenebase.h"
#include "../http/httpresponse.h"
#include "../http/firstline.h"
#include "../utils/log.h"
#include "../http/headerfield.h"

NetSceneBase::NetSceneBase()
        : status_code_(200)
        , status_desc_("OK")
        , socket_(-1) {
    http_headers_[http::HeaderField::KContentType] = http::HeaderField::KOctetStream;
    http_headers_[http::HeaderField::KConnection] = http::HeaderField::KConnectionClose;
    
}

void NetSceneBase::SetSocket(SOCKET _socket) {
    if (_socket > 0) { socket_ = _socket; }
}


int NetSceneBase::DoScene(const std::string &_in_buffer) {
    int ret = DoSceneImpl(_in_buffer);
    PackAndSend();
    return ret;
}

void NetSceneBase::CopyRespToSendBody(std::string &_resp, size_t _size) {
    send_body_.Reset();
    LogI("[type%d CopyRespToSendBody] resp body len = %zd", GetType(), _size);
    send_body_.Write(_resp.data(), _size);
}

void NetSceneBase::PackAndSend() {
    AutoBuffer out_buff;
    http::response::Pack(http::kHTTP_1_1, status_code_,
                         status_desc_, http_headers_, out_buff, send_body_);
    LogI("[NetSceneBase::PackAndSend] send len: %ld", out_buff.Length())
//    __ShowHttpHeader(out_buff);
    send_body_.Reset();
    // TODO: replace send with block socket send
    send(socket_, out_buff.Ptr(), out_buff.Length(), 0);
    
}

void NetSceneBase::__ShowHttpHeader(AutoBuffer &_out) {
    for (size_t i = 0; i < _out.Length() - send_body_.Length(); ++i) {
        if (*_out.Ptr(i) == 0x0d || *_out.Ptr(i) == 0x0a) {
            LogI("0x%x ", *_out.Ptr(i))
        } else {
            LogI("0x%x %c", *_out.Ptr(i), *_out.Ptr(i))
        }
    }
}
