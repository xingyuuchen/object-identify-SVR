#pragma once
#include "netsceneprotobuf.h"
#include "dbitem/dbitem_userinfo.h"
#include "netscenechangenickname.pb.h"


class NetSceneChangeNickname : public NetSceneProtobuf {
  public:
    
    NetSceneChangeNickname();
    
    int GetType() override;
    
    NetSceneBase *NewInstance() override;
    
    int DoSceneImpl(const std::string &_in_buffer) override;
    
    RespMessage *GetRespMessage() override;

  private:
    NetSceneChangeNicknameProto::NetSceneChangeNicknameResp resp_;
    
};

