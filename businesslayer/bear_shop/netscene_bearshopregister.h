#pragma once
#include "netscenecustom.h"
#include <mutex>
#include <string>


class NetSceneBearShopRegister : public NetSceneCustom {
  public:
    NetSceneBearShopRegister();
    
    void *Data() override;
    
    size_t Length() override;
    
    int GetType() override;
    
    const char *Route() override;
    
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
