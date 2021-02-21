#include "netscenedispatcher.h"
#include <stdio.h>
#include "../autogen/basenetscenereq.pb.h"
#include "../utils/log.h"
#include "netscene_getindexpage.h"
#include "netscene_queryimg.h"
#include "netscene_gettrainprogress.h"


NetSceneDispatcher::NetSceneDispatcher() {
    std::unique_lock<std::mutex> lock(mutex_);
    selectors_.push_back(new NetSceneGetIndexPage());
    selectors_.push_back(new NetSceneQueryImg());
    selectors_.push_back(new NetSceneGetTrainProgress());
    
}

NetSceneDispatcher::~NetSceneDispatcher() {
    std::unique_lock<std::mutex> lock(mutex_);
    for (std::vector<NetSceneBase *>::iterator iter = selectors_.begin(); iter != selectors_.end(); iter++) {
        delete *iter;
    }
}

int NetSceneDispatcher::Dispatch(SOCKET _conn_fd, const AutoBuffer *_in_buffer) {
    LogI("[Dispatch] _in_buffer.len: %zd", _in_buffer->Length());
    
    if (_in_buffer->Ptr() == NULL) {
        LogI("[Dispatch] _in_buffer->Ptr() == NULL, return index page.")
        NetSceneGetIndexPage net_scene;
        net_scene.SetSocket(_conn_fd);
        return net_scene.DoScene("");
    }
    BaseNetSceneReq::BaseNetSceneReq base_req;
    base_req.ParseFromArray(_in_buffer->Ptr(), _in_buffer->Length());
    
    if (base_req.has_net_scene_type()) {
        int type = base_req.net_scene_type();
        {
            std::unique_lock<std::mutex> lock(mutex_);
            for (auto iter = selectors_.begin(); iter != selectors_.end(); iter++) {
                if ((*iter)->GetType() == type) {
                    if (base_req.has_net_scene_req_buff()) {
                        std::string req_buffer = base_req.net_scene_req_buff();
                
                        NetSceneBase *net_scene = (*iter)->NewInstance();
                
                        net_scene->SetSocket(_conn_fd);
                        int ret = net_scene->DoScene(req_buffer);
                
                        delete net_scene;
                
                        return ret;
                    } else {
                        LogI("[Dispatch] type:%d, base_req.has_net_scene_req_buff(): false", type)
                        return -1;
                    }
                }
            }
        }
        LogE("NO such NetScene: type=%d, give up processing this request.", type);
    }
    
    return -1;
}
