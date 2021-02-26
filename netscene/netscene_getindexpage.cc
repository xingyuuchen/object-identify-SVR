#include "netscene_getindexpage.h"
#include "../constantsprotocol.h"
#include "../http/headerfield.h"


NetSceneGetIndexPage::NetSceneGetIndexPage() : NetSceneBase() {
    http_headers_[http::HeaderField::KContentType] = http::HeaderField::KPlainText;
}

int NetSceneGetIndexPage::GetType() { return kIndexPage; }

std::shared_ptr<NetSceneBase> NetSceneGetIndexPage::NewInstance() {
    return std::make_shared<NetSceneGetIndexPage>();
}

int NetSceneGetIndexPage::DoSceneImpl(const std::string &_in_buffer) {
    static int visit_times_since_last_boot_ = 0;
    std::string resp = "If you see this, the server is running normally, "
                   + std::to_string(++visit_times_since_last_boot_) + " visits since last boot.";
    CopyRespToSendBody(resp, resp.size());
    return 0;
}
