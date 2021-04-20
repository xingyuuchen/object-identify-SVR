#include "log.h"
#include "server.h"
#include "netscenedispatcher.h"
#include "businesslayer/registry.h"
#ifdef DAEMON
#include "daemon.h"
#endif


int main(int ac, char **argv) {
#ifdef DAEMON
    if (Daemon::Daemonize() < 0) {
        printf("Daemonize failed\n");
        return 0;
    }
#endif
    
    logger::OpenLog(argv[0]);

    LogI("Launching Server...")
    
    registry::RegisterNetScenes();
    
    Server::Instance().SetWorker<NetSceneDispatcher::NetSceneWorker>();

    Server::Instance().Serve();
    
    LogI("Server Down")
    return 0;
}

