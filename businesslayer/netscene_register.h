#ifndef OI_SVR_NETSCENE_REGISTER_H
#define OI_SVR_NETSCENE_REGISTER_H
#include "netscenebase.h"


class NetSceneRegister : public NetSceneBase {
  public:
    NetSceneRegister();
    
    int GetType() override;
    
    NetSceneBase *NewInstance() override;
    
    int DoSceneImpl(const std::string &_in_buffer) override;

  private:
  
};


#endif //OI_SVR_NETSCENE_REGISTER_H
