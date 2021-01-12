#ifndef OI_SVR_NETSCENE_QUERYIMG_H
#define OI_SVR_NETSCENE_QUERYIMG_H

#include <string>
#include "../netscenebase.h"
#include "../autobuffer.h"


class NetSceneQueryImg : public NetSceneBase {
  public:
    enum ItemType {
        kTypePlant      = 0,
        kTypeAnimal     = 1,
        kTypeLandMark   = 2,
    };
    
    NetSceneQueryImg();
    
    ~NetSceneQueryImg();
    
    int GetType();
    
    int DoScene(const AutoBuffer& _buffer);

  public:
  
  
  private:
    std::string item_name_;
    std::string item_desc_;
    ItemType    item_type_;
    
};


#endif //OI_SVR_NETSCENE_QUERYIMG_H
