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
    
    const char *Route() override;
    
    NetSceneBase *NewInstance() override;
    
    int DoSceneImpl(const std::string &_in_buffer) override;
    
    const char *ContentType() override;

  private:
    static const char *const    kUrlRoute;
    static const char *const    kBaseDirPath;
    char *                      content_type_;
    std::string                 resp_;
};