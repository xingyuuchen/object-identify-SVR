#include "netscene_uploadavatar.h"
#include "netscenetypes.h"
#include "log.h"
#include "constantsprotocol.h"
#include "fileutil.h"
#include "dbitem/dbitem_userinfo.h"
#include "connection.h"


int NetSceneUploadAvatar::last_file_seq_when_boot_ = 0;

const char *const NetSceneUploadAvatar::avatar_saved_file_name_ = "/root/cxy/avatars/%d.avatar";

const char *const NetSceneUploadAvatar::cmd_find_last_seq_ = "ls -t /root/cxy/avatars | head -1";

std::mutex NetSceneUploadAvatar::mutex_;

NetSceneUploadAvatar::NetSceneUploadAvatar()
        : NetSceneBase() {
    
    NETSCENE_INIT_START
        __LoadFileSeqNum();
    NETSCENE_INIT_END
}

int NetSceneUploadAvatar::GetType() { return kNetSceneTypeUploadAvatar; }

NetSceneBase *NetSceneUploadAvatar::NewInstance() { return new NetSceneUploadAvatar(); }

NetSceneBase::RespMessage *NetSceneUploadAvatar::GetRespMessage() { return &resp_; }

int NetSceneUploadAvatar::DoSceneImpl(const std::string &_in_buffer) {
    LogI(__FILE__, "[DoSceneImpl] req.size: %zd", _in_buffer.size());
    
    NetSceneUploadAvatarProto::NetSceneUploadAvatarReq req;
    req.ParseFromArray(_in_buffer.data(), _in_buffer.size());
    uint32_t usr_id = req.usr_id();
    std::string avatar_img_bytes = req.avatar_img_bytes();
    size_t avatar_bytes_len = avatar_img_bytes.length();
    
    do {
        if (usr_id <= 0 || avatar_img_bytes.empty()) {
            LogI(__FILE__, "[DoSceneImpl] usr_id: %d, avatar_bytes empty: %d",
                 usr_id, avatar_img_bytes.empty())
            errcode_ = kErrIllegalReq;
            errmsg_ = "usr_id or avatar is illegal.";
            break;
        }
        int seq = __MakeFileSeq();
        char file_path[32] = {0, };
        {
            std::unique_lock<std::mutex> lock(mutex_);
            snprintf(file_path, sizeof(file_path), avatar_saved_file_name_, seq);
        }
    
        size_t size = File::WriteFileBin(file_path, avatar_img_bytes.data(), avatar_bytes_len);
        
        if (size < avatar_bytes_len) {
            LogE(__FILE__, "[DoSceneImpl] write: %ld, total: %ld", size, avatar_bytes_len)
            errmsg_ = "fwrite your avatar failed.";
            errcode_ = kErrFileBroken;
            break;
        }
        LogI(__FILE__, "[DoSceneImpl] write file succeed")
    
        DBItem_UserInfo old;
        old.SetUsrId(usr_id);
        DBItem_UserInfo neo;
        neo.SetAvatarPath(std::string(file_path));
        Dao::Update(old, neo);
        int db_ret = Dao::Update(old, neo);
        if (db_ret < 0) {
            LogI(__FILE__, "[DoSceneImpl] db err, usrid: %d, file_path: %s",
                 usr_id, file_path)
            errcode_ = kErrDatabase;
            errmsg_ = "db err.";
        }
        LogI(__FILE__, "[DoSceneImpl] update db succeed")
        
    } while (false);
    
    resp_.set_nop(true);
    
    return 0;
}

int NetSceneUploadAvatar::__MakeFileSeq() {
    // do synchronization
    std::unique_lock<std::mutex> lock(mutex_);
    return ++last_file_seq_when_boot_;
}

void NetSceneUploadAvatar::__LoadFileSeqNum() {
    FILE *fp = popen(cmd_find_last_seq_, "r");
    
    char ret[32] = {0, };
    fgets(ret, sizeof(ret), fp);
    pclose(fp);
    
    LogI(__FILE__, "[__LoadFileSeqNum] ret: %s", ret)
    for (size_t i = 0; ret[i]; ++i) {
        if (ret[i] == '.') {
            ret[i] = 0;
            last_file_seq_when_boot_ = (int) strtol(ret, 0, 10);
            LogI(__FILE__, "[__LoadFileSeqNum] last_file_seq_when_boot_: %d",
                    last_file_seq_when_boot_)
            return;
        }
    }
}

