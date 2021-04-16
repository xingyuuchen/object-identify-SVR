#pragma once
#include "netscenebase.h"
#include <mutex>
#include "../netscene_getfile.h"
#include <string>


class NetSceneBearShopLogin : public NetSceneBase {
  public:
    NetSceneBearShopLogin();
    
    bool IsUseProtobuf() override;
    
    void *Data() override;
    
    size_t Length() override;
    
    int GetType() override;
    
    char *Route() override;
    
    NetSceneBase *NewInstance() override;
    
    int DoSceneImpl(const std::string &_in_buffer) override;

  private:
    static const char *const    kUrlRoute;
    static const char *const    kLoginErrResp;
    static const char *const    kWrongPwdResp;
    char *                      resp_;
};
