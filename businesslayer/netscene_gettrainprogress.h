#ifndef OI_SVR_NETSCENE_GETTRAINPROGRESS_H
#define OI_SVR_NETSCENE_GETTRAINPROGRESS_H

#include "netscenebase.h"
#include "autobuffer.h"

/**
 * 业务代码，获取目前训练进度。
 */
class NetSceneGetTrainProgress : public NetSceneBase {
  public:
    
    NetSceneGetTrainProgress();
    
    ~NetSceneGetTrainProgress() override;
    
    int GetType() override;
    
    NetSceneBase *NewInstance() override;
    
    int DoSceneImpl(const std::string &_in_buffer) override;
    
    
  private:
    bool        is_running_;
    int32_t     curr_epoch_;
    int32_t     total_epoch_;
    
};


#endif //OI_SVR_NETSCENE_GETTRAINPROGRESS_H
