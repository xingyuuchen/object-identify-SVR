#include "netscenedispatcher.h"
#include <stdio.h>
#include "../autogen/basenetscenereq.pb.h"
#include "../utils/log.h"
#include "netscene_getindexpage.h"
#include "netscene_queryimg.h"
#include "netscene_gettrainprogress.h"


const char *const NetSceneDispatcher::TAG = "NetSceneDispatcher";

NetSceneDispatcher::NetSceneDispatcher() {
    std::unique_lock<std::mutex> lock(mutex_);
    selectors_.push_back(new NetSceneGetIndexPage());
    selectors_.push_back(new NetSceneQueryImg());
    selectors_.push_back(new NetSceneGetTrainProgress());
    
}

NetSceneBase *NetSceneDispatcher::__MakeNetScene(int _type) {
    std::unique_lock<std::mutex> lock(mutex_);
    auto iter = std::find_if(selectors_.begin(), selectors_.end(),
                        [=] (NetSceneBase *find) { return find->GetType() == _type; });
    if (iter == selectors_.end()) {
        LogE(TAG, "[__MakeNetScene] NO such NetScene:"
             " type=%d, give up processing this request.", _type)
        return NULL;
    }
    return (*iter)->NewInstance();
}


/**
 * Dispatches the request to corresponding net_scene,
 * waiting for the latter finished processing.
 *
 * @return
 */
NetSceneBase *NetSceneDispatcher::Dispatch(SOCKET _conn_fd, const AutoBuffer *_in_buffer) {
    int type;
    std::string req_buffer;
    do {
        if (_in_buffer == NULL || _in_buffer->Ptr() == NULL) {
            LogI(TAG, "[Dispatch] return index page.")
            type = 0;
            break;
        }
        LogI(TAG, "[Dispatch] _in_buffer.len: %zd", _in_buffer->Length());
    
        BaseNetSceneReq::BaseNetSceneReq base_req;
        base_req.ParseFromArray(_in_buffer->Ptr(), _in_buffer->Length());
    
        if (!base_req.has_net_scene_type()) {
            LogI(TAG, "[Dispatch] base_req.has_net_scene_type(): false")
            return NULL;
        }
        type = base_req.net_scene_type();
        if (!base_req.has_net_scene_req_buff()) {
            LogI(TAG, "[Dispatch] type(%d), base_req.has_net_scene_req_buff(): false", type)
            return NULL;
        }
        req_buffer = base_req.net_scene_req_buff();
    } while (false);
    
    NetSceneBase *net_scene = __MakeNetScene(type);
    if (net_scene) {
        net_scene->SetSocket(_conn_fd);
        net_scene->DoScene(req_buffer);
    }
    return net_scene;
}

NetSceneDispatcher::~NetSceneDispatcher() {
    std::unique_lock<std::mutex> lock(mutex_);
    for (auto iter = selectors_.begin(); iter != selectors_.end(); iter++) {
        delete *iter;
    }
}
