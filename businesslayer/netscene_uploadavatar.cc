#include "netscene_uploadavatar.h"
#include "constantsprotocol.h"
#include "log.h"

NetSceneUploadAvatar::NetSceneUploadAvatar() : NetSceneBase() {

}

int NetSceneUploadAvatar::GetType() { return kNetSceneTypeUploadAvatar; }

NetSceneBase *NetSceneUploadAvatar::NewInstance() { return new NetSceneUploadAvatar(); }

int NetSceneUploadAvatar::DoSceneImpl(const std::string &_in_buffer) {
    return 0;
}


