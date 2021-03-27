#include "netscenedispatcher.h"
#include <stdio.h>
#include "basenetscenereq.pb.h"
#include "netscene_getindexpage.h"
#include "netscene_hellosvr.h"
#include "log.h"
#include "http/httpresponse.h"


NetSceneDispatcher::NetSceneDispatcher() {
    std::unique_lock<std::mutex> lock(mutex_);
    selectors_.push_back(new NetSceneGetIndexPage());
    selectors_.push_back(new NetSceneHelloSvr());
    
}

void NetSceneDispatcher::RegisterNetScene(NetSceneBase *_net_scene) {
    std::unique_lock<std::mutex> lock(mutex_);
    if (_net_scene) {
        selectors_.push_back(_net_scene);
    }
}


NetSceneBase *NetSceneDispatcher::__MakeNetScene(int _type) {
    std::unique_lock<std::mutex> lock(mutex_);
    auto iter = std::find_if(selectors_.begin(), selectors_.end(),
                        [=] (NetSceneBase *find) { return find->GetType() == _type; });
    if (iter == selectors_.end()) {
        LogE(__FILE__, "[__MakeNetScene] NO such NetScene:"
             " type=%d, give up processing this request.", _type)
        return NULL;
    }
    return (*iter)->NewInstance();
}

NetSceneDispatcher::NetSceneWorker::~NetSceneWorker() {

}

void NetSceneDispatcher::NetSceneWorker::HandleImpl(Tcp::RecvContext *_recv_ctx) {
    if (!_recv_ctx) {
        return;
    }
    if (!net_thread_) {
        LogE(__FILE__, "[HandleImpl] wtf? No net_thread!")
        return;
    }
    SOCKET fd = _recv_ctx->fd;
    AutoBuffer &buffer = _recv_ctx->buffer;
    
    int type;
    std::string req_buffer;
    do {
        if (!buffer.Ptr()) {
            LogI(__FILE__, "[HandleImpl] return index page.")
            type = 0;
            break;
        }
        LogI(__FILE__, "[HandleImpl] _in_buffer.len: %zd", buffer.Length());
    
        BaseNetSceneReq::BaseNetSceneReq base_req;
        base_req.ParseFromArray(buffer.Ptr(), buffer.Length());
    
        if (!base_req.has_net_scene_type()) {
            LogI(__FILE__, "[HandleImpl] base_req.has_net_scene_type(): false")
            return;
        }
        type = base_req.net_scene_type();
        if (!base_req.has_net_scene_req_buff()) {
            LogI(__FILE__, "[HandleImpl] type(%d), base_req.has_net_scene_req_buff(): false", type)
            return;
        }
        req_buffer = base_req.net_scene_req_buff();
    } while (false);
    
    LogI(__FILE__, "[HandleImpl] dispatch to type %d", type)
    
    NetSceneBase *net_scene = NetSceneDispatcher::Instance().__MakeNetScene(type);
    if (!net_scene) {
        LogE(__FILE__, "[HandleImpl] !net_scene")
        return;
    }
    net_scene->SetSocket(fd);
    uint64_t start = ::gettickcount();
    net_scene->DoScene(req_buffer);
    LogI(__FILE__, "[HandleImpl] type:%d, cost: %llu ms", type, ::gettickcount() - start)
    
    AutoBuffer &http_resp_msg = _recv_ctx->send_context->buffer;
    __PackHttpResp(net_scene, http_resp_msg);
    
    delete net_scene, net_scene = nullptr;
    
    Server::SendQueue *send_queue = net_thread_->GetSendQueue();
    
    send_queue->push(_recv_ctx->send_context);
    net_thread_->NotifyEpoll();
    
}

void NetSceneDispatcher::NetSceneWorker::HandleException(std::exception &ex) {
    LogE(__FILE__, "[HandleException] %s", ex.what())
    running_ = false;
}

void NetSceneDispatcher::NetSceneWorker::__PackHttpResp(
        NetSceneBase *_net_scene, AutoBuffer &_http_msg) {
    
    std::map<std::string, std::string> headers;
    
    if (_net_scene->UseProtobuf()) {
        headers[http::HeaderField::KContentType] = http::HeaderField::KOctetStream;
    } else {
        headers[http::HeaderField::KContentType] = http::HeaderField::KPlainText;
    }
    headers[http::HeaderField::KConnection] = http::HeaderField::KConnectionClose;
    
    std::string status_desc = "OK";
    int resp_code = 200;
    
    http::response::Pack(http::kHTTP_1_1, resp_code,status_desc,
                         headers, _http_msg, _net_scene->GetHttpBody());
    
}


NetSceneDispatcher::~NetSceneDispatcher() {
    std::unique_lock<std::mutex> lock(mutex_);
    for (auto iter = selectors_.begin(); iter != selectors_.end(); iter++) {
        delete *iter, *iter = NULL;
    }
}
