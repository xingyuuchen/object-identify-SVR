#include "netscene_queryimg.h"
#include "../netscenetypes.h"
#include "log.h"
#include "signalhandler.h"
#include "strutil.h"
#include <string>
#include <atomic>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

/**
 * 业务代码，查询图片类别。
 */

const char *const NetSceneQueryImg::fifo_name_ = "/tmp/fifo_queryimg";

NetSceneQueryImg::NetSceneQueryImg() : NetSceneBase() {
    static std::atomic_flag has_init = ATOMIC_FLAG_INIT;
    if (!has_init.test_and_set()) {
        if (__MakeFIFO() == 0) {
            SignalHandler::Instance().RegisterExitCallback(
                    new IProcessExitListener([] { __DelFIFO(); }));
        }
    }
    item_type_ = NetSceneQueryImgProto::NetSceneQueryImgResp::PLANT;
    item_name_ = "查询有误";
    item_desc_ = "请稍后再试。";
}

int NetSceneQueryImg::GetType() { return kNetSceneTypeQueryImg; }

NetSceneBase *NetSceneQueryImg::NewInstance() { return new NetSceneQueryImg(); }

int NetSceneQueryImg::DoSceneImpl(const std::string &_in_buffer) {
    LogI(__FILE__, "[DoSceneImpl] req.len: %zd", _in_buffer.size());
    if (socket_ <= 0) {
        LogE(__FILE__, "[NetSceneQueryImg] Socket NOT open");
        return -1;
    }
    NetSceneQueryImgProto::NetSceneQueryImgReq req;
    req.ParseFromArray(_in_buffer.data(), _in_buffer.size());
    
    do {
        char py_ret[200] = {0, };
        if (__ForkPythonScript((char *)req.img_bytes().c_str(),
                               req.img_bytes().size(), py_ret, sizeof(py_ret)) < 0) {
            LogE(__FILE__, "[DoSceneImpl] __ForkPythonScript() failed.")
            break;
        }
        std::vector<std::string> item_info;
        oi::split(std::string(py_ret), "$", item_info);
        if (item_info.size() != 3) {
            LogE(__FILE__, "[DoSceneImpl] item_info.size(): %zd", item_info.size())
            break;
        }
        if (strcmp(item_info[1].c_str(), "None") == 0) {
            item_name_ = "未识别到物体";
            item_desc_ = "";
            break;
        }
        int type = item_info[0].c_str()[0] - 0x30;
        type = (type >= 0 && type <= 2) ? type : 0;
        item_type_ = (NetSceneQueryImgProto::NetSceneQueryImgResp_ItemType) type;
        item_name_ = item_info[1];
        item_desc_ = item_info[2];
    } while (false);
    
    NetSceneQueryImgProto::NetSceneQueryImgResp resp;
    resp.set_item_name(item_name_);
    resp.set_item_type(item_type_);
    resp.set_item_desc(item_desc_);
    
    size_t size = resp.ByteSizeLong();
    std::string byte_string;
    resp.SerializeToString(&byte_string);
    Write2BaseResp(byte_string, size);
    return 0;
}

int NetSceneQueryImg::__ForkPythonScript(char *_data_write, size_t _size_write,
                                         char *_ret, size_t _size_ret) {
    FILE *fp;
    std::string cmd = "python3 ../businesslayer/netscene_queryimg/queryimg.py "
                + std::string(fifo_name_) + " " + std::to_string(_size_write);
    if ((fp = popen(cmd.c_str(), "r")) == NULL) {
        LogE(__FILE__, "[__ForkPythonScript] popen err")
        return -1;
    }
    // FIXME: no concurrency
    int fd = open(fifo_name_, O_WRONLY);
    if (fd < 0) {
        LogE(__FILE__, "[__ForkPythonScript] open err(%d): %s", errno, strerror(errno))
        pclose(fp);
        return -1;
    }
    LogI(__FILE__, "[__ForkPythonScript] open fifo success, fd: %d", fd)
    
    ssize_t nwrite = ::write(fd, _data_write, _size_write);
    LogI(__FILE__, "[__ForkPythonScript] write %zd/%zd", nwrite, _size_write)
    
    close(fd);
    fgets(_ret, _size_ret, fp);
    pclose(fp);
    LogI(__FILE__, "[__ForkPythonScript] _out: %s, len: %ld\n", _ret, strlen(_ret));
    return 0;
}

int NetSceneQueryImg::__MakeFIFO() {
    if (mkfifo(fifo_name_, 0777) < 0) {
        LogE(__FILE__, "[__MakeFIFO] mkfifo err(%d): %s", errno, strerror(errno))
        return -1;
    }
    LogI(__FILE__, "[__MakeFIFO] success")
    return 0;
}

void NetSceneQueryImg::__DelFIFO() {
    if (unlink(fifo_name_) < 0) {
        LogE(__FILE__, "[__DelFIFO] unlink err(%d): %s", errno, strerror(errno))
        return;
    }
    LogI(__FILE__, "[__DelFIFO] success")
}

NetSceneQueryImg::~NetSceneQueryImg() {

}
