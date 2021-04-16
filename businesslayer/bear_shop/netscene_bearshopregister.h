#pragma once
#include "netscenebase.h"
#include <mutex>
#include <string>


class NetSceneBearShopRegister : public NetSceneBase {
  public:
    NetSceneBearShopRegister();
    
    bool IsUseProtobuf() override;
    
    void *Data() override;
    
    size_t Length() override;
    
    int GetType() override;
    
    char *Route() override;
    
    NetSceneBase *NewInstance() override;
    
    int DoSceneImpl(const std::string &_in_buffer) override;
    
    static const char *GetBearShopRedirectResp();

  private:
    static const char *const    kUrlRoute;
    static const char *const    kRegisterFailedResp;
    static const char *const    kUsernameAlreadyUsedResp;
    static const char *const    kRedirectToBearShopResp;
    char *                      resp_;

};
