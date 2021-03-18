#include "log.h"
#include "httpserver.h"
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
    
    HttpServer::Instance().Run();
    
    return 0;
}

