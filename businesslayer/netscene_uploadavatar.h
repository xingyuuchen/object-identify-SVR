#ifndef OI_SVR_NETSCENE_UPLOADAVATAR_H
#define OI_SVR_NETSCENE_UPLOADAVATAR_H
#include "netscenebase.h"
#include <mutex>


class NetSceneUploadAvatar : public NetSceneBase {
  public:
    NetSceneUploadAvatar();
    
    int GetType() override;
    
    NetSceneBase *NewInstance() override;
    
    int DoSceneImpl(const std::string &_in_buffer) override;

  private:
    static void __LoadFileSeqNum();
    
    static int __MakeFileSeq();
    
  private:
    static const char *const    avatar_saved_file_name_;
    static const char *const    cmd_find_last_seq_;
    static int                  last_file_seq_when_boot_;
    static std::mutex           mutex_;
    
};


#endif //OI_SVR_NETSCENE_UPLOADAVATAR_H
