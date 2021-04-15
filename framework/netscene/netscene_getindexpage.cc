#include "netscene_getindexpage.h"
#include "constantsprotocol.h"
#include "log.h"

const char *const NetSceneGetIndexPage::kUrlRoute = "/";

const char *const NetSceneGetIndexPage::kRespFormat =
        "If you see this, the server is running normally, %d visits since last boot.";

std::mutex NetSceneGetIndexPage::mutex_;

NetSceneGetIndexPage::NetSceneGetIndexPage()
        : NetSceneBase() {
}

int NetSceneGetIndexPage::GetType() { return kNetSceneTypeGetIndexPage; }

NetSceneBase *NetSceneGetIndexPage::NewInstance() { return new NetSceneGetIndexPage(); }

int NetSceneGetIndexPage::DoSceneImpl(const std::string &_in_buffer) {
    static int visit_times_since_last_boot_ = 0;
    
    std::unique_lock<std::mutex> lock(mutex_);
    
    snprintf(resp_, sizeof(resp_), kRespFormat, ++visit_times_since_last_boot_);
    
    return 0;
}


void *NetSceneGetIndexPage::Data() {
    return resp_;
}

size_t NetSceneGetIndexPage::Length() {
    LogI("%lu", strlen(resp_))
    return strlen(resp_);
}

bool NetSceneGetIndexPage::IsUseProtobuf() {
    return false;
}

char *NetSceneGetIndexPage::Route() {
    return const_cast<char *>(kUrlRoute);
}
