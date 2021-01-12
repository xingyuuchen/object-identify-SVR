#include "netscene_queryimg.h"
#include "../constantsprotocol.h"
#include <string>

NetSceneQueryImg::NetSceneQueryImg() : NetSceneBase() {

}

int NetSceneQueryImg::GetType() {
    return kNetSceneTypeQueryImg;
}

int NetSceneQueryImg::DoScene(const AutoBuffer& _buffer) {
    printf("%s\n", _buffer.Ptr());
    if (socket_ <= 0) {
        printf("socket not open\n");
        return -1;
    }
    // todo: recognize by _buffer
    
    item_type_ = kTypePlant;
    item_name_ = "猪笼草";
    item_desc_ = "这是来着svr的猪笼草信息...";
    
    send_body_.Reset();
    
    send_body_.Write((unsigned char *) &item_type_, sizeof(item_type_));
    send_body_.Write((unsigned char *) item_name_.c_str(), item_name_.length());
    send_body_.Write((unsigned char *) item_desc_.c_str(), item_desc_.length());
    send(socket_, send_body_.Ptr(), send_body_.Length(), 0);
    
    return 0;
}

NetSceneQueryImg::~NetSceneQueryImg() {

}
