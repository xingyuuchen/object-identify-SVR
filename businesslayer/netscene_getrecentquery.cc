#include "netscene_getrecentquery.h"
#include "netscenetypes.h"


NetSceneGetRecentQuery::NetSceneGetRecentQuery()
        : NetSceneProtobuf() {
}

int NetSceneGetRecentQuery::GetType() { return kNetSceneTypeGetRecentQuery; }

NetSceneBase *NetSceneGetRecentQuery::NewInstance() { return new NetSceneGetRecentQuery(); }

NetSceneProtobuf::RespMessage *NetSceneGetRecentQuery::GetRespMessage() { return nullptr; }

int NetSceneGetRecentQuery::DoSceneImpl(const std::string &_in_buffer) {
    
    
    return 0;
}

