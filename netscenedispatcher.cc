#include "netscenedispatcher.h"
#include <stdio.h>
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

int NetSceneDispatcher::Dispatch(SOCKET _conn_fd, const AutoBuffer *_buffer) {
    Log("[Dispatch] recv: %s, len: %ld", _buffer->Ptr(), _buffer->Length());
    int type = 0;   // TODO: decode
    
    for (auto iter = selectors_.begin(); iter != selectors_.end(); iter++) {
        if ((*iter)->GetType() == type) {
            (*iter)->SetSocket(_conn_fd);
            return (*iter)->DoScene(*_buffer);
        }
    }
    Log("NO such NetScene: type: %d", type);
    return -1;
}
