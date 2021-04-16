#include "netscene_getfile.h"
#include "netscenetypes.h"
#include "fileutil.h"
#include "log.h"
#include <string>


const char *const NetSceneGetFile::kBaseDirPath = "/root/cxy/staticfiles/";

const char *const NetSceneGetFile::kUrlRoute = "/file/*";

NetSceneGetFile::NetSceneGetFile()
        : NetSceneBase() {
}

bool NetSceneGetFile::IsUseProtobuf() { return false; }

void *NetSceneGetFile::Data() { return (void *) resp_.data(); }

size_t NetSceneGetFile::Length() { return resp_.size(); }

int NetSceneGetFile::GetType() { return kNetSceneTypeGetFile; }

char *NetSceneGetFile::Route() { return const_cast<char *>(kUrlRoute); }

NetSceneBase *NetSceneGetFile::NewInstance() { return new NetSceneGetFile(); }

int NetSceneGetFile::DoSceneImpl(const std::string &_in_buffer) {
    
    std::string file_path = std::string(kBaseDirPath) +
            _in_buffer.substr(strlen(kUrlRoute) - 1);
    LogI("file_path: %s", file_path.c_str())
    
    if (!file::IsFileExist(file_path.c_str())) {
        resp_ = "404. Resource Not Found";
        return -1;
    }
    if (!file::ReadFile(file_path.c_str(), resp_)) {
        LogE("read login_html failed")
        resp_ = "Internal Server Error, Contact xingyuuchen for help.";
        return -1;
    }
    return 0;
}
