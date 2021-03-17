#ifndef OI_SVR_NETSCENE_GETRECENTQUERY_H
#define OI_SVR_NETSCENE_GETRECENTQUERY_H
#include "netscenebase.h"


/**
 *
 */
class NetSceneGetRecentQuery : public NetSceneBase {
  public:
    NetSceneGetRecentQuery();
  
    int GetType() override;
    
    NetSceneBase *NewInstance() override;
    
    int DoSceneImpl(const std::string &_in_buffer) override;

  private:
  
};


#endif //OI_SVR_NETSCENE_GETRECENTQUERY_H
