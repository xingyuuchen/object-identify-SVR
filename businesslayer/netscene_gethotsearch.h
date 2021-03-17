#ifndef OI_SVR_NETSCENE_GETHOTSEARCH_H
#define OI_SVR_NETSCENE_GETHOTSEARCH_H
#include "netscenebase.h"


class NetSceneGetHotSearch : public NetSceneBase {
  public:
    
    NetSceneGetHotSearch();
    
    int GetType() override;
    
    NetSceneBase *NewInstance() override;
    
    int DoSceneImpl(const std::string &_in_buffer) override;

  private:
  
};


#endif //OI_SVR_NETSCENE_GETHOTSEARCH_H
