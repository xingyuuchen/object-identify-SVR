#include "netscene_gettrainprogress.h"
#include "../log.h"
#include "../constantsprotocol.h"
#include "../autogen/netscenegettrainprogress.pb.h"


NetSceneGetTrainProgress::NetSceneGetTrainProgress() {

}

NetSceneGetTrainProgress::~NetSceneGetTrainProgress() { }

int NetSceneGetTrainProgress::GetType() { return kNetSceneTypeGetTrainProgress; }


int NetSceneGetTrainProgress::DoSceneImpl(const std::string &_in_buffer) {
    LogI("[NetSceneGetTrainProgress::DoSceneImpl] req.len: %zd", _in_buffer.size());
    if (socket_ <= 0) {
        LogE("[NetSceneGetTrainProgress] Socket NOT open");
        return -1;
    }
    
    send_body_.Reset();
    NetSceneGetTrainProgressProto::NetSceneGetTrainProgressResp resp;
    resp.set_is_running(true);
    resp.set_curr_epoch(1);
    resp.set_total_epoch(5);
    
    size_t size = resp.ByteSizeLong();
    std::string byte_string;
    resp.SerializeToString(&byte_string);
    
    CopyRespToSendBody(byte_string, size);
    return 0;
    
}


