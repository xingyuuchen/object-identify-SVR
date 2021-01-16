#include "netscenedispatcher.h"
#include <stdio.h>
#include "autogen/basenetscenereq.pb.h"
#include "netscene/netscene_queryimg.h"
#include "log.h"


NetSceneDispatcher::NetSceneDispatcher() {
    selectors_.push_back(new NetSceneQueryImg());
    
    
}

NetSceneDispatcher::~NetSceneDispatcher() {
    for (std::vector<NetSceneBase *>::iterator iter = selectors_.begin(); iter != selectors_.end(); iter++) {
        delete *iter;
    }
}

int NetSceneDispatcher::Dispatch(SOCKET _conn_fd, const AutoBuffer *_in_buffer) {
    Log("[Dispatch], len: %ld", _in_buffer->Length());
    
    BaseNetSceneReq::BaseNetSceneReq base_req;
    base_req.ParseFromArray(_in_buffer->Ptr(), _in_buffer->Length());
    
    int type = base_req.net_scene_type();
    std::string req_buffer = base_req.net_scene_req_buff();
    
    for (auto iter = selectors_.begin(); iter != selectors_.end(); iter++) {
        if ((*iter)->GetType() == type) {
            (*iter)->SetSocket(_conn_fd);
            return (*iter)->DoScene(req_buffer);
        }
    }
    Log("NO such NetScene: type: %d", type);
    return -1;
}
