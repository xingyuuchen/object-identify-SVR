#ifndef OI_SVR_NETSCENE_GETTRAINPROGRESS_H
#define OI_SVR_NETSCENE_GETTRAINPROGRESS_H

#include "../netscenebase.h"
#include "../autobuffer.h"


class NetSceneGetTrainProgress : public NetSceneBase {
  public:
    
    NetSceneGetTrainProgress();
    
    ~NetSceneGetTrainProgress();
    
    int GetType();
    
    int DoSceneImpl(const std::string &_in_buffer);
    
    
  private:
    uint32_t    curr_epoch_;
    uint32_t    total_epoch_;
    bool        is_running_;
    
};


#endif //OI_SVR_NETSCENE_GETTRAINPROGRESS_H
