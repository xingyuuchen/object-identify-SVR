#include "log.h"
#include "webserver.h"
#include "businesslayer/registry.h"
#ifdef DAEMON
#include "daemon.h"
#endif


int main(int ac, char **argv) {
#ifdef DAEMON
    if (unixtar::Daemonize() < 0) {
        printf("Daemonize failed\n");
        return 0;
    }
#endif
    
    logger::OpenLog(argv[0]);

    WebServer::Instance().Config();
    
    registry::RegisterNetScenes();
    
    LogI("Launching Webserver...")

    WebServer::Instance().Serve();
    
    LogI("Webserver Down")
    return 0;
}

