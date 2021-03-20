#include "netscene_uploadavatar.h"
#include "netscenetypes.h"
#include "log.h"
#include "netsceneuploadavatar.pb.h"
#include "constantsprotocol.h"
#include "fileutil.h"
#include "db/userinfo.h"
#include "connection.h"
#include <atomic>


int NetSceneUploadAvatar::last_file_seq_when_boot_ = 0;

const char *const NetSceneUploadAvatar::avatar_saved_file_name_ = "/root/cxy/avatars/%d.avatar";

const char *const NetSceneUploadAvatar::cmd_find_last_seq_ = "ls -t /root/cxy/avatars | head -1";

NetSceneUploadAvatar::NetSceneUploadAvatar()
        : NetSceneBase() {
    static std::atomic_flag has_init = ATOMIC_FLAG_INIT;
    if (!has_init.test_and_set()) {
        __LoadFileSeqNum();
    }
}

int NetSceneUploadAvatar::GetType() { return kNetSceneTypeUploadAvatar; }

NetSceneBase *NetSceneUploadAvatar::NewInstance() { return new NetSceneUploadAvatar(); }

int NetSceneUploadAvatar::DoSceneImpl(const std::string &_in_buffer) {
    LogI(__FILE__, "[DoSceneImpl] req.size: %zd", _in_buffer.size());
    if (socket_ <= 0) {
        LogE(__FILE__, "[NetSceneUploadAvatar] Socket NOT open");
        return -1;
    }
    NetSceneUploadAvatarProto::NetSceneUploadAvatarReq req;
    req.ParseFromArray(_in_buffer.data(), _in_buffer.size());
    uint32_t usr_id = req.usr_id();
    std::string avatar_img_bytes = req.avatar_img_bytes();
    size_t avatar_bytes_len = avatar_img_bytes.length();
    
    do {
        if (usr_id <= 0 || avatar_img_bytes.empty()) {
            LogI(__FILE__, "[DoSceneImpl] usr_id: %d, avatar_bytes empty: %d",
                 usr_id, avatar_img_bytes.empty())
            base_resp_.set_errmsg("usr_id or avatar is illegal.");
            base_resp_.set_errcode(kErrIllegalReq);
            break;
        }
        int seq = __MakeFileSeq();
        char file_path[32] = {0, };
        {
//            std::unique_lock<std::mutex> lock(mutex_);
            snprintf(file_path, sizeof(file_path), avatar_saved_file_name_, seq);
        }
    
        size_t size = File::WriteFileBin(file_path, avatar_img_bytes.data(), avatar_bytes_len);
        
        if (size < avatar_bytes_len) {
            LogE(__FILE__, "[DoSceneImpl] write: %ld, total: %ld", size, avatar_bytes_len)
            base_resp_.set_errmsg("fwrite your avatar failed.");
            base_resp_.set_errcode(kErrFileBroken);
            break;
        }
        LogI(__FILE__, "[DoSceneImpl] write file succeed")
    
        UserInfo old;
        old.SetUsrId(usr_id);
        UserInfo neo;
        neo.SetAvatarPath(std::string(file_path));
        Dao::Update(old, neo);
        int db_ret = Dao::Update(old, neo);
        if (db_ret < 0) {
            LogI(__FILE__, "[DoSceneImpl] db err, usrid: %d, file_path: %s",
                 usr_id, file_path)
            base_resp_.set_errcode(kErrDatabase);
            base_resp_.set_errmsg("db err.");
        }
        LogI(__FILE__, "[DoSceneImpl] update db succeed")
        
    } while (false);
    
    NetSceneUploadAvatarProto::NetSceneUploadAvatarResp resp;
    resp.set_nop(true);
    
    return 0;
}

int NetSceneUploadAvatar::__MakeFileSeq() {
    // do synchronization
//    std::unique_lock<std::mutex> lock(mutex_);
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

