#include "netscene_getcovid19infected.h"
#include "log.h"
#include "../netscenetypes.h"
#include "timeutil.h"
#include "fileutil.h"
#include "threadpool/threadpool.h"


const char *const NetSceneGetCovid19Infected::kUrlRoute = "/wjk-world-covid19-infected";

const char *const NetSceneGetCovid19Infected::kHtmlPath =
        "/root/cxy/staticfiles/wjk/world_map.html";

std::mutex NetSceneGetCovid19Infected::html_mtx_;

NetSceneGetCovid19Infected::NetSceneGetCovid19Infected()
        : NetSceneCustom() {
    NETSCENE_INIT_START
        ThreadPool::Instance().ExecutePeriodic(2 * 60 * 60 * 1000,
                   [] { NetSceneGetCovid19Infected::__GenerateCovid19InfectedHtml(); });
    NETSCENE_INIT_END
}

void *NetSceneGetCovid19Infected::Data() { return (void *) resp_.data(); }

size_t NetSceneGetCovid19Infected::Length() { return resp_.size(); }

NetSceneGetCovid19Infected::~NetSceneGetCovid19Infected() = default;

int NetSceneGetCovid19Infected::GetType() { return kNetSceneTypeGetCovid19Infected; }

NetSceneBase *NetSceneGetCovid19Infected::NewInstance() { return new NetSceneGetCovid19Infected(); }

int NetSceneGetCovid19Infected::DoSceneImpl(const std::string &_in_buffer) {
    {
        std::lock_guard<std::mutex> lock(html_mtx_);
        if (!file::ReadFile(kHtmlPath, resp_)) {
            LogI("read html failed")
            resp_ = "Internal Server Error! Contact Jason Wang for help.";
            return -1;
        }
    }
    return 0;
}

const char *NetSceneGetCovid19Infected::Route() { return kUrlRoute; }

int NetSceneGetCovid19Infected::__GenerateCovid19InfectedHtml() {
    uint64_t start = ::gettickcount();
    FILE *fp;
    std::string cmd = "python3 /root/cxy/staticfiles/wjk/map.py";
    {
        std::lock_guard<std::mutex> lock(html_mtx_);
        if (!(fp = popen(cmd.c_str(), "r"))) {
            LogE("popen err")
            return -1;
        }
        pclose(fp);
    }
    int cost = (int) (::gettickcount() - start);
    LogI("python cost %d ms", cost)
    return 0;
}
