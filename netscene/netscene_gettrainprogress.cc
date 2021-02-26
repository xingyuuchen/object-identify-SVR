#include "netscene_gettrainprogress.h"
#include "../utils/log.h"
#include "../constantsprotocol.h"
#include "../autogen/netscenegettrainprogress.pb.h"
#include <fstream>

/**
 * 业务代码，获取目前训练进度。
 */
NetSceneGetTrainProgress::NetSceneGetTrainProgress()
    : NetSceneBase()
    , curr_epoch_(-1)
    , total_epoch_(-1)
    , is_running_(false) {}

NetSceneGetTrainProgress::~NetSceneGetTrainProgress() { }

int NetSceneGetTrainProgress::GetType() { return kNetSceneTypeGetTrainProgress; }

NetSceneBase *NetSceneGetTrainProgress::NewInstance() { return new NetSceneGetTrainProgress(); }

int NetSceneGetTrainProgress::DoSceneImpl(const std::string &_in_buffer) {
    LogI("[NetSceneGetTrainProgress::DoSceneImpl] req.len: %zd", _in_buffer.size());
    if (socket_ <= 0) {
        LogE("[NetSceneGetTrainProgress] Socket NOT open");
        return -1;
    }
    
    NetSceneGetTrainProgressProto::NetSceneGetTrainProgressResp resp;
    std::ifstream infile("/root/cxy/trainprogress.txt");
    if (infile) {
        infile >> total_epoch_;
        curr_epoch_ = 0;
        float hit_rate;
        while (infile >> hit_rate) {
            curr_epoch_++;
            resp.add_hit_rates(hit_rate);
        }
        is_running_ = true;
        infile.close();
    } else {
        status_desc_ = "/root/cxy/trainprogress.txt-FileNotOpen";
        status_code_ = 404;
        LogE("[NetSceneGetTrainProgress::DoSceneImpl] infile.is_open() = false")
    }
    LogI("[NetSceneGetTrainProgress] isRunning:%d, currEpoch:%d, totalEpoch:%d",
         is_running_, curr_epoch_, total_epoch_)
    
    resp.set_is_running(is_running_);
    resp.set_curr_epoch(curr_epoch_);
    resp.set_total_epoch(total_epoch_);
    
    size_t size = resp.ByteSizeLong();
    std::string byte_string;
    resp.SerializeToString(&byte_string);
    
    CopyRespToSendBody(byte_string, size);
    return 0;
    
}


