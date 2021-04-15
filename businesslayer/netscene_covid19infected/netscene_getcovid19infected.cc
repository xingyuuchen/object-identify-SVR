#include "netscene_getcovid19infected.h"
#include "log.h"
#include "../netscenetypes.h"
#include "timeutil.h"


const char *const NetSceneGetCovid19Infected::kUrlRoute = "/wjk-world-covid19-infected";

const char *const NetSceneGetCovid19Infected::kHtmlPath = "/root/cxy/wjk/world_map.html";

std::mutex NetSceneGetCovid19Infected::html_mtx_;

NetSceneGetCovid19Infected::NetSceneGetCovid19Infected()
        : NetSceneBase() {
}

void *NetSceneGetCovid19Infected::Data() { return resp_.Ptr(); }

size_t NetSceneGetCovid19Infected::Length() { return resp_.Length(); }

NetSceneGetCovid19Infected::~NetSceneGetCovid19Infected() = default;

int NetSceneGetCovid19Infected::GetType() { return kNetSceneTypeGetCovid19Infected; }

NetSceneBase *NetSceneGetCovid19Infected::NewInstance() { return new NetSceneGetCovid19Infected(); }

int NetSceneGetCovid19Infected::DoSceneImpl(const std::string &_in_buffer) {
    FILE *fp = nullptr;
    do {
        if (__GenerateCovid19InfectedHtml() < 0) {
            LogE("generate html failed")
            break;
        }
        uint64_t start = ::gettickcount();
        {
            std::lock_guard<std::mutex> lock(html_mtx_);
            fp = ::fopen(kHtmlPath, "r");
            if (!fp) {
                LogE("open file failed, path: %s, errno(%d): %s",
                     kHtmlPath, errno, strerror(errno));
                break;
            }
            fseek(fp, 0, SEEK_END);
            long file_size = ::ftell(fp);
            if (file_size <= 0) {
                LogE("file_size: %lu", file_size)
                break;
            }
            resp_.AddCapacity(file_size);
            fseek(fp, 0, SEEK_SET);
            int ret = fread(resp_.Ptr(), file_size, 1, fp);
            if (ret != 1) {
                LogE("fread ret: %d", ret)
                break;
            }
            ::fclose(fp);
            resp_.SetLength(file_size);
        }
        int cost = (int) (::gettickcount() - start);
        LogI("read html cost %d ms", cost)
        return 0;
    } while (false);
    
    if (fp) {
        ::fclose(fp);
    }
    std::string bad_resp = "Internal Server Error";
    resp_.Write(bad_resp.data(), bad_resp.size());
    
    return -1;
}

bool NetSceneGetCovid19Infected::IsUseProtobuf() { return false; }

char *NetSceneGetCovid19Infected::Route() { return const_cast<char *>(kUrlRoute); }

int NetSceneGetCovid19Infected::__GenerateCovid19InfectedHtml() {
    uint64_t start = ::gettickcount();
    FILE *fp;
    std::string cmd = "python3 /root/cxy/wjk/map.py";
    if (!(fp = popen(cmd.c_str(), "r"))) {
        LogE("popen err")
        return -1;
    }
    pclose(fp);
    int cost = (int) (::gettickcount() - start);
    LogI("python cost %d ms", cost)
    return 0;
}
