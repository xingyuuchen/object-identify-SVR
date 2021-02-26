#ifndef OI_SVR_NETSCENE_GETINDEXPAGE_H
#define OI_SVR_NETSCENE_GETINDEXPAGE_H

#include "netscenebase.h"


/**
 * this netScene uses plain-text instead of protobuf.
 */
class NetSceneGetIndexPage : public NetSceneBase {
  public:
    NetSceneGetIndexPage();
    
    int GetType();
    
    virtual std::shared_ptr<NetSceneBase> NewInstance();
    
    int DoSceneImpl(const std::string &_in_buffer);
    
  private:
  
};


#endif //OI_SVR_NETSCENE_GETINDEXPAGE_H
