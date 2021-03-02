#ifndef OI_SVR_NETSCENE_QUERYIMG_H
#define OI_SVR_NETSCENE_QUERYIMG_H

#include <string>
#include "../netscenebase.h"
#include "../../utils/autobuffer.h"
#include "../../autogen/netscenequeryimg.pb.h"

/**
 * 业务代码，查询图片类别。
 */
class NetSceneQueryImg : public NetSceneBase {
  public:
    
    NetSceneQueryImg();
    
    ~NetSceneQueryImg();
    
    int GetType();
    
    NetSceneBase *NewInstance();
    
    int DoSceneImpl(const std::string &_in_buffer);
    
  private:
    int __ForkPythonScript(char *_data_write, size_t _write_len, char *_out);
    
    static int __MakeFIFO();
    
    static void __DelFIFO();
    
  private:
    static const char *const TAG;
    static const char *const fifo_name_;
    std::string item_name_;
    std::string item_desc_;
    NetSceneQueryImgProto::NetSceneQueryImgResp_ItemType item_type_;
    
};


#endif //OI_SVR_NETSCENE_QUERYIMG_H
