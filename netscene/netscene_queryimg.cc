#include "netscene_queryimg.h"
#include "../constantsprotocol.h"
#include <string>
#include "../log.h"


NetSceneQueryImg::NetSceneQueryImg() : NetSceneBase() {

}

int NetSceneQueryImg::GetType() {
    return kNetSceneTypeQueryImg;
}

int NetSceneQueryImg::DoSceneImpl(const std::string &_in_buffer) {
    LogI("[NetSceneQueryImg::DoSceneImpl] req.len: %zd", _in_buffer.size());
    if (socket_ <= 0) {
        LogI("Socket NOT open");
        return -1;
    }
    NetSceneQueryImgProto::NetSceneQueryImgReq req;
    req.ParseFromArray(_in_buffer.data(), _in_buffer.size());
    // TODO: recognize by _buffer
    
    item_type_ = NetSceneQueryImgProto::NetSceneQueryImgResp::PLANT;
    item_name_ = "豌豆尖";
    item_desc_ = "可以结出豌豆，和瘦肉一起炒很好吃";
    
    NetSceneQueryImgProto::NetSceneQueryImgResp resp;
    resp.set_item_name(item_name_);
    resp.set_item_type(item_type_);
    resp.set_item_desc(item_desc_);
    
    size_t size = resp.ByteSizeLong();
    std::string byte_string;
    resp.SerializeToString(&byte_string);
    
    send_body_.Reset();
    LogI("[NetSceneQueryImg::DoSceneImpl] resp body len = %zd", size);
    send_body_.Write(byte_string.data(), size);
    
    return 0;
}

NetSceneQueryImg::~NetSceneQueryImg() {

}
