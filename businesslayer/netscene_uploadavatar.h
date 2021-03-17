#ifndef OI_SVR_NETSCENE_UPLOADAVATAR_H
#define OI_SVR_NETSCENE_UPLOADAVATAR_H
#include "netscenebase.h"


class NetSceneUploadAvatar : public NetSceneBase {
  public:
    NetSceneUploadAvatar();
    
    int GetType() override;
    
    NetSceneBase *NewInstance() override;
    
    int DoSceneImpl(const std::string &_in_buffer) override;

  private:
};


#endif //OI_SVR_NETSCENE_UPLOADAVATAR_H
