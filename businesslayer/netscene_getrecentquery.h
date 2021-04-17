#ifndef OI_SVR_NETSCENE_GETRECENTQUERY_H
#define OI_SVR_NETSCENE_GETRECENTQUERY_H
#include "netsceneprotobuf.h"


/**
 *
 */
class NetSceneGetRecentQuery : public NetSceneProtobuf {
  public:
    NetSceneGetRecentQuery();
  
    int GetType() override;
    
    NetSceneBase *NewInstance() override;
    
    int DoSceneImpl(const std::string &_in_buffer) override;
    
    RespMessage *GetRespMessage() override;

  private:
  
};


#endif //OI_SVR_NETSCENE_GETRECENTQUERY_H
