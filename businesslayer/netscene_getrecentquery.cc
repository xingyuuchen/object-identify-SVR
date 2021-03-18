#include "netscene_getrecentquery.h"
#include "netscenetypes.h"


NetSceneGetRecentQuery::NetSceneGetRecentQuery() : NetSceneBase() {

}

int NetSceneGetRecentQuery::GetType() { return kNetSceneTypeGetRecentQuery; }

NetSceneBase *NetSceneGetRecentQuery::NewInstance() { return new NetSceneGetRecentQuery(); }

int NetSceneGetRecentQuery::DoSceneImpl(const std::string &_in_buffer) {
    return 0;
}

