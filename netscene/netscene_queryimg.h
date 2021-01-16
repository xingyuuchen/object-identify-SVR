#ifndef OI_SVR_NETSCENE_QUERYIMG_H
#define OI_SVR_NETSCENE_QUERYIMG_H

#include <string>
#include "../netscenebase.h"
#include "../autobuffer.h"
#include "../autogen/netscenequeryimg.pb.h"


class NetSceneQueryImg : public NetSceneBase {
  public:
    
    NetSceneQueryImg();
    
    ~NetSceneQueryImg();
    
    int GetType();
    
    int DoScene(const std::string &_in_buffer);

  public:
  
  
  private:
    std::string item_name_;
    std::string item_desc_;
    NetSceneQueryImgProto::NetSceneQueryImgResp_ItemType item_type_;
    
};


#endif //OI_SVR_NETSCENE_QUERYIMG_H
