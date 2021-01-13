#include "netscene_queryimg.h"
#include "../constantsprotocol.h"
#include <string>
#include "../log.h"


NetSceneQueryImg::NetSceneQueryImg() : NetSceneBase() {

}

int NetSceneQueryImg::GetType() {
    return kNetSceneTypeQueryImg;
}

int NetSceneQueryImg::DoScene(const AutoBuffer& _buffer) {
    Log("[NetSceneQueryImg::DoScene] %s", _buffer.Ptr());
    if (socket_ <= 0) {
        Log("socket not open");
        return -1;
    }
    // todo: recognize by _buffer
    
    item_type_ = kTypePlant;
    item_name_ = "Grass";
    item_desc_ = "This is grass message from svr...";
    
    send_body_.Reset();
    
    send_body_.Write((unsigned char *) &item_type_, sizeof(item_type_));
    send_body_.Write((unsigned char *) item_name_.c_str(), item_name_.length());
    send_body_.Write((unsigned char *) item_desc_.c_str(), item_desc_.length());
    send(socket_, send_body_.Ptr(), send_body_.Length(), 0);
    
    return 0;
}

NetSceneQueryImg::~NetSceneQueryImg() {

}
