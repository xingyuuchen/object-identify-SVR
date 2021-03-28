#include "netscene_gettrainprogress.h"
#include "log.h"
#include "netscenetypes.h"
#include "constantsprotocol.h"
#include <fstream>


/**
 * 业务代码，获取目前训练进度。
 */

NetSceneGetTrainProgress::NetSceneGetTrainProgress()
    : NetSceneBase()
    , curr_epoch_(-1)
    , total_epoch_(-1)
    , is_running_(false) {}

NetSceneGetTrainProgress::~NetSceneGetTrainProgress() = default;

int NetSceneGetTrainProgress::GetType() { return kNetSceneTypeGetTrainProgress; }

NetSceneBase *NetSceneGetTrainProgress::NewInstance() { return new NetSceneGetTrainProgress(); }

NetSceneBase::RespMessage *NetSceneGetTrainProgress::GetRespMessage() { return &resp_; }

int NetSceneGetTrainProgress::DoSceneImpl(const std::string &_in_buffer) {
    LogI(__FILE__, "[DoSceneImpl] req.len: %zd", _in_buffer.size());
    
    std::ifstream infile("/root/cxy/trainprogress.txt");
    if (infile) {
        infile >> total_epoch_;
        curr_epoch_ = 0;
        float hit_rate;
        while (infile >> hit_rate) {
            curr_epoch_++;
            resp_.add_hit_rates(hit_rate);
        }
        is_running_ = true;
        infile.close();
    } else {
        errcode_ = kErrFileBroken;
        errmsg_ = "/root/cxy/trainprogress.txt-FileNotOpen";
        LogE(__FILE__, "[DoSceneImpl] infile.is_open() = false")
    }
    LogI(__FILE__, "[DoSceneImpl] isRunning:%d, currEpoch:%d, totalEpoch:%d",
         is_running_, curr_epoch_, total_epoch_)
    
    resp_.set_is_running(is_running_);
    resp_.set_curr_epoch(curr_epoch_);
    resp_.set_total_epoch(total_epoch_);
    
    return 0;
    
}


