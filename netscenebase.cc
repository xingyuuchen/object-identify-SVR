#include "netscenebase.h"
#include "http/httpresponse.h"
#include "http/firstline.h"
#include "log.h"


void NetSceneBase::SetSocket(SOCKET _socket) {
    if (_socket > 0) { socket_ = _socket; }
}


int NetSceneBase::DoScene(const std::string &_in_buffer) {
    int ret = DoSceneImpl(_in_buffer);
    PackAndSend();
    return ret;
}


void NetSceneBase::PackAndSend() {
    AutoBuffer out_buff;
    std::map <std::string, std::string> empty;
    std::string status_desc("OK");
    http::response::Pack(http::kHTTP_1_1, 200,
                         status_desc, empty, out_buff, send_body_);
    LogI("[NetSceneBase::PackAndSend] send len: %ld", out_buff.Length())
//    for (int i = 0; i < out_buff.Length() - send_body_.Length(); ++i) {
//        if (*out_buff.Ptr(i) == 0x0d || *out_buff.Ptr(i) == 0x0a) {
//            LogI("0x%x ", *out_buff.Ptr(i))
//        } else {
//            LogI("0x%x %c", *out_buff.Ptr(i), *out_buff.Ptr(i))
//        }
//    }
    send_body_.Reset();
    send(socket_, out_buff.Ptr(), out_buff.Length(), 0);
    
}
