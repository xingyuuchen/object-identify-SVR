#include "netscene_getindexpage.h"
#include "../constantsprotocol.h"
#include "../http/headerfield.h"


NetSceneGetIndexPage::NetSceneGetIndexPage() : NetSceneBase() {
    http_headers_[http::HeaderField::KContentType] = http::HeaderField::KPlainText;
}

int NetSceneGetIndexPage::GetType() { return kIndexPage; }

NetSceneBase * NetSceneGetIndexPage::NewInstance() { return new NetSceneGetIndexPage(); }

int NetSceneGetIndexPage::DoSceneImpl(const std::string &_in_buffer) {
    std::string resp = "If you see this, the server is running normally.";
    CopyRespToSendBody(resp, resp.size());
    return 0;
}
