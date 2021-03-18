#include "netscene_gethotsearch.h"
#include "netscenetypes.h"


NetSceneGetHotSearch::NetSceneGetHotSearch() : NetSceneBase() {

}

int NetSceneGetHotSearch::GetType() { return kNetSceneTypeGetHotSearch; }

NetSceneBase *NetSceneGetHotSearch::NewInstance() { return new NetSceneGetHotSearch(); }

int NetSceneGetHotSearch::DoSceneImpl(const std::string &_in_buffer) {
    return 0;
}
