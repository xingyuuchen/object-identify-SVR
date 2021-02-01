#include "netscene_gettrainprogress.h"
#include "../log.h"
#include "../constantsprotocol.h"
#include "../autogen/netscenegettrainprogress.pb.h"
#include "../strutil.h"

/**
 * 业务代码，获取目前训练进度。
 */
NetSceneGetTrainProgress::NetSceneGetTrainProgress()
    : curr_epoch_(-1)
    , total_epoch_(-1)
    , is_running_(false) {}

NetSceneGetTrainProgress::~NetSceneGetTrainProgress() { }

int NetSceneGetTrainProgress::GetType() { return kNetSceneTypeGetTrainProgress; }


int NetSceneGetTrainProgress::DoSceneImpl(const std::string &_in_buffer) {
    LogI("[NetSceneGetTrainProgress::DoSceneImpl] req.len: %zd", _in_buffer.size());
    if (socket_ <= 0) {
        LogE("[NetSceneGetTrainProgress] Socket NOT open");
        return -1;
    }
    
    FILE* file = fopen("/root/cxy/trainprogress.txt", "r");
    if (file) {
        do {
            char ch[10] = {0, };
            fread(ch, sizeof(char), sizeof(ch), file);
            fclose(file);
            std::vector<std::string> res;
            oi::split(ch, " ", res);
            if (res.size() != 2) {
                LogE("[NetSceneGetTrainProgress::DoSceneImpl] res.size=%zd", res.size())
                break;
            }
            is_running_ = true;
            curr_epoch_ = std::stoi(res[0]);
            total_epoch_ = std::stoi(res[1]);
        } while (false);
    }
    LogI("[NetSceneGetTrainProgress] isRunning:%d, currEpoch:%d, totalEpoch:%d",
         is_running_, curr_epoch_, total_epoch_)
    
    send_body_.Reset();
    NetSceneGetTrainProgressProto::NetSceneGetTrainProgressResp resp;
    resp.set_is_running(is_running_);
    resp.set_curr_epoch(curr_epoch_);
    resp.set_total_epoch(total_epoch_);
    
    size_t size = resp.ByteSizeLong();
    std::string byte_string;
    resp.SerializeToString(&byte_string);
    
    CopyRespToSendBody(byte_string, size);
    return 0;
    
}


