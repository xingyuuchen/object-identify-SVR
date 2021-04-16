#pragma once
#include "netscenebase.h"
#include "../netscene_getfile.h"
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
    
    static std::string &GetBearShopHtmlPath();

  private:
    static const char *const    kUrlRoute;
    static const char *const    kRegisterFailedResp;
    static std::string          kBearShopHtmlPath;
    bool                        register_success_;
    NetSceneGetFile             net_scene_get_file_;
//    static const char *const    kLoginHtmlPath;

};
