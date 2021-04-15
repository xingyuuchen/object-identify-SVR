#include "netscenebase.h"
#include "log.h"
#include "headerfield.h"
#include "socketepoll.h"
#include "constantsprotocol.h"
#include <errno.h>


NetSceneBase::NetSceneBase() {
    errcode_ = kOK;
    errmsg_ = "OK";
    
}


int NetSceneBase::DoScene(const std::string &_in_buffer) {
    int ret = DoSceneImpl(_in_buffer);

    RespMessage *resp = GetRespMessage();
    if (IsUseProtobuf() && resp) {
        base_resp_.set_errcode(errcode_);
        base_resp_.set_errmsg(errmsg_);
        base_resp_.set_net_scene_resp_buff(resp->SerializeAsString());
    }
    
    if (IsUseProtobuf()) {
        base_resp_.SerializeToString(&resp_buffer_);
    } else {
        resp_buffer_ = std::string((const char *) Data(), Length());
    }
//    __ShowHttpHeader(out_buff);
    return ret;
}


void NetSceneBase::__ShowHttpHeader(AutoBuffer &_out) {
    for (size_t i = 0; i < _out.Length(); ++i) {
        if (*_out.Ptr(i) == 0x0d || *_out.Ptr(i) == 0x0a) {
            LogI("0x%x ", *_out.Ptr(i))
        } else {
            LogI("0x%x %c", *_out.Ptr(i), *_out.Ptr(i))
        }
    }
}

std::string &NetSceneBase::GetRespBuffer() { return resp_buffer_; }

bool NetSceneBase::IsUseProtobuf() { return true; }

NetSceneBase::RespMessage *NetSceneBase::GetRespMessage() { return nullptr; }

void *NetSceneBase::Data() { return nullptr; }

size_t NetSceneBase::Length() { return 0; }

char *NetSceneBase::Route() { return nullptr; }

