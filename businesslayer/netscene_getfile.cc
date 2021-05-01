#include "netscene_getfile.h"
#include "netscenetypes.h"
#include "netscene_404notfound.h"
#include "fileutil.h"
#include "log.h"
#include <string>
#include "http/headerfield.h"


const char *const NetSceneGetFile::kBaseDirPath = "/root/cxy/staticfiles/";

const char *const NetSceneGetFile::kUrlRoute = "/file/*";

NetSceneGetFile::NetSceneGetFile()
        : NetSceneCustom()
        , content_type_(nullptr) {
}

void *NetSceneGetFile::Data() { return (void *) resp_.data(); }

size_t NetSceneGetFile::Length() { return resp_.size(); }

int NetSceneGetFile::GetType() { return kNetSceneTypeGetFile; }

const char *NetSceneGetFile::Route() { return kUrlRoute; }

const char *NetSceneGetFile::ContentType() {
    return content_type_ ? content_type_ : NetSceneCustom::ContentType();
}

NetSceneBase *NetSceneGetFile::NewInstance() { return new NetSceneGetFile(); }

int NetSceneGetFile::DoSceneImpl(const std::string &_in_buffer) {
    
    std::string file_path = std::string(kBaseDirPath) +
            _in_buffer.substr(strlen(kUrlRoute) - 1);
    LogI("file_path: %s", file_path.c_str())
    
    if (!file::IsFileExist(file_path.c_str())) {
        LogI("file(%s) NOT exist", file_path.c_str())
        NetScene404NotFound _404;
        resp_ = std::string((char *) _404.Data());
        return 0;
    }
    if (!file::ReadFile(file_path.c_str(), resp_)) {
        LogE("read file(%s) failed", file_path.c_str())
        resp_ = "Internal Server Error, Contact xingyuuchen for help.";
        return -1;
    }
    std::string suffix = file_path.substr(file_path.rfind('.') + 1);
    if (suffix == "html" || suffix == "HTML" ||
            suffix == "htm" || suffix == "HTM") {
        content_type_ = (char *) http::HeaderField::kTextHtml;
    } else if (suffix == "css" || suffix == "CSS") {
        content_type_ = (char *) http::HeaderField::kTextCss;
    } else if (suffix == "jpg" || suffix == "JPG" ||
                suffix == "jpeg" || suffix == "JPEG") {
        content_type_ = (char *) http::HeaderField::kImageJpg;
    } else if (suffix == "png" || suffix == "PNG") {
        content_type_ = (char *) http::HeaderField::kImagePng;
    }
    return 0;
}
