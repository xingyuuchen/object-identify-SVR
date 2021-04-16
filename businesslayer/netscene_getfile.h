#pragma once
#include "netscenebase.h"
#include <mutex>
#include <string>


class NetSceneGetFile : public NetSceneBase {
  public:
    NetSceneGetFile();
    
    bool IsUseProtobuf() override;
    
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