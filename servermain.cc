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
    
    Logger::OpenLog(argv[0]);

    LogI(__FILE__, "Launching SVR...")
    
    Registry::RegisterNetScenes();
    
    Server::Instance().SetWorker<NetSceneDispatcher::NetSceneWorker>();

    Server::Instance().Serve();
    
    return 0;
}

