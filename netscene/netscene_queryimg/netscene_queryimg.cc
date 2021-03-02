#include "netscene_queryimg.h"
#include "../constantsprotocol.h"
#include "../../utils/log.h"
#include "../../signalhandler.h"
#include <string>
#include <atomic>
#include <sys/stat.h>

/**
 * 业务代码，查询图片类别。
 */

const char *const NetSceneQueryImg::TAG = "NetSceneQueryImg";

NetSceneQueryImg::NetSceneQueryImg() : NetSceneBase() {
    static std::atomic_flag has_init = ATOMIC_FLAG_INIT;
    if (!has_init.test_and_set()) {
        if (__MakeFIFO() == 0) {
            SignalHandler::Instance().RegisterExitCallback(
                    new IProcessExitListener([] { DelFIFO(); }));
        }
    }
}

int NetSceneQueryImg::GetType() { return kNetSceneTypeQueryImg; }

NetSceneBase *NetSceneQueryImg::NewInstance() { return new NetSceneQueryImg(); }

int NetSceneQueryImg::DoSceneImpl(const std::string &_in_buffer) {
    LogI(TAG, "[DoSceneImpl] req.len: %zd", _in_buffer.size());
    if (socket_ <= 0) {
        LogE(TAG, "[NetSceneQueryImg] Socket NOT open");
        return -1;
    }
    NetSceneQueryImgProto::NetSceneQueryImgReq req;
    req.ParseFromArray(_in_buffer.data(), _in_buffer.size());
    
    char line[100] = "豌豆尖";
//    FILE *fp;
////    if ((fp = popen("pwd", "r")) == NULL) {
//    if ((fp = popen("python3 ../netscene/netscene_queryimg/queryimgfrombaidu.py", "r")) == NULL) {
//        printf("err\n");
//        return -1;
//    }
//    fgets(line, sizeof(line), fp);
//    pclose(fp);
    LogI(TAG, "line: %s, len: %ld\n", line, strlen(line));
    
    item_type_ = NetSceneQueryImgProto::NetSceneQueryImgResp::PLANT;
    item_name_ = line;
    item_desc_ = "可以结出豌豆，和瘦肉一起炒很好吃";
    
    NetSceneQueryImgProto::NetSceneQueryImgResp resp;
    resp.set_item_name(item_name_);
    resp.set_item_type(item_type_);
    resp.set_item_desc(item_desc_);
    
    size_t size = resp.ByteSizeLong();
    std::string byte_string;
    resp.SerializeToString(&byte_string);
    CopyRespToSendBody(byte_string, size);
    return 0;
}

int NetSceneQueryImg::__MakeFIFO() {
    LogI(TAG, "[__MakeFIFO]")
//    int ret = mkfifo("fifo_queryimg", 777);
    
    return ret;
}

void NetSceneQueryImg::DelFIFO() {
    LogI(TAG, "[DelFIFO]")
    
}

NetSceneQueryImg::~NetSceneQueryImg() {

}
