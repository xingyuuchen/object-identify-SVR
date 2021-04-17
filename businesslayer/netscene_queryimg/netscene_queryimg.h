#ifndef OI_SVR_NETSCENE_QUERYIMG_H
#define OI_SVR_NETSCENE_QUERYIMG_H

#include <string>
#include <mutex>
#include "netsceneprotobuf.h"
#include "autobuffer.h"
#include "netscenequeryimg.pb.h"

/**
 * 业务代码，查询图片类别。
 */
class NetSceneQueryImg : public NetSceneProtobuf {
  public:
    
    NetSceneQueryImg();
    
    ~NetSceneQueryImg() override;
    
    int GetType() override;
    
    RespMessage *GetRespMessage() override;
    
    NetSceneBase *NewInstance() override;
    
    int DoSceneImpl(const std::string &_in_buffer) override;
    
    static uint64_t GetLastQueryTs();
    
  private:
    static int __ForkPythonScript(char *_data_write, size_t _size_write,
                           char *_ret, size_t _size_ret);
    
    static int __MakeFIFO();
    
    static void __DelFIFO();
    
  private:
    static uint64_t             last_query_ts_;
    static std::mutex           last_query_ts_mtx_;
    static const char *const    fifo_name_;
    std::string                 item_name_;
    std::string                 item_desc_;
    NetSceneQueryImgProto::NetSceneQueryImgResp resp_;
    NetSceneQueryImgProto::NetSceneQueryImgResp_ItemType item_type_;
    
};


#endif //OI_SVR_NETSCENE_QUERYIMG_H
