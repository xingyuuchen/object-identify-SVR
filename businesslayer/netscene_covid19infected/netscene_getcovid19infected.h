#pragma once
#include "netscenebase.h"
#include "autobuffer.h"
#include <mutex>


class NetSceneGetCovid19Infected : public NetSceneBase {
  public:
    
    NetSceneGetCovid19Infected();
    
    void *Data() override;
    
    size_t Length() override;
    
    ~NetSceneGetCovid19Infected() override;
    
    int GetType() override;
    
    NetSceneBase *NewInstance() override;
    
    bool IsUseProtobuf() override;
    
    char *Route() override;
    
    int DoSceneImpl(const std::string &_in_buffer) override;
    
  private:
    static int __GenerateCovid19InfectedHtml();
    
  private:
    static const char * const   kUrlRoute;
    static const char * const   kHtmlPath;
    static std::mutex           html_mtx_;
    AutoBuffer                  resp_;
};

