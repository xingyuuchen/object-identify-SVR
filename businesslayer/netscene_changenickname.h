#pragma once
#include "netscenebase.h"
#include "db/userinfo.h"


class NetSceneChangeNickname : public NetSceneBase {
  public:
    
    NetSceneChangeNickname();
    
    int GetType() override;
    
    NetSceneBase *NewInstance() override;
    
    int DoSceneImpl(const std::string &_in_buffer) override;

  private:
  
};

