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
    LogI("[Dispatch] _in_buffer.len: %zd", _in_buffer->Length());
    
    BaseNetSceneReq::BaseNetSceneReq base_req;
    base_req.ParseFromArray(_in_buffer->Ptr(), _in_buffer->Length());
    
    if (base_req.has_net_scene_type()) {
        int type = base_req.net_scene_type();
        for (auto iter = selectors_.begin(); iter != selectors_.end(); iter++) {
            if ((*iter)->GetType() == type) {
                if (base_req.has_net_scene_req_buff()) {
                    std::string req_buffer = base_req.net_scene_req_buff();
                    (*iter)->SetSocket(_conn_fd);
                    return (*iter)->DoScene(req_buffer);
                } else {
                    LogI("[Dispatch] type:%d, base_req.has_net_scene_req_buff(): false", type)
                    return -1;
                }
            }
        }
        LogI("ERR: NO such NetScene: type: %d, did not processing this request.", type);
    } else {
        LogI("[Dispatch] base_req.has_net_scene_type(): false")
    }
    
    return -1;
}
