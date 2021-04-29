#pragma once
#include "netscenebase.h"
#include <mutex>
#include "../netscene_getfile.h"
#include <string>


class NetSceneBearShopLogin : public NetSceneCustom {
  public:
    NetSceneBearShopLogin();
    
    void *Data() override;
    
    size_t Length() override;
    
    int GetType() override;
    
    const char *Route() override;
    
    NetSceneBase *NewInstance() override;
    
    const char *ContentType() override;
    
    int DoSceneImpl(const std::string &_in_buffer) override;

  private:
    static const char *const    kUrlRoute;
    static const char *const    kLoginErrResp;
    static const char *const    kWrongPwdResp;
    char *                      resp_;
};
