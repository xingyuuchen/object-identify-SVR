#pragma once
#include "netscenecustom.h"
#include <mutex>
#include <string>


class NetSceneGetFile : public NetSceneCustom {
  public:
    NetSceneGetFile();
    
    void *Data() override;
    
    size_t Length() override;
    
    int GetType() override;
    
    char *Route() override;
    
    NetSceneBase *NewInstance() override;
    
    int DoSceneImpl(const std::string &_in_buffer) override;

  private:
    static const char *const    kUrlRoute;
    static const char *const    kBaseDirPath;
    std::string                 resp_;
};