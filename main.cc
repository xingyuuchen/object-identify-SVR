#include "log.h"
#include "httpserver.h"
#ifdef DAEMON
#include "daemon.h"
#endif


int main(int ac, char **argv) {
#ifdef DAEMON
    if (Daemon::Daemonize() < 0) {
        return 0;
    }
#endif
    
    Logger::OpenLog(argv[0]);

    LogI("main", "Launching SVR...")
    
    HttpServer::Instance().Run();
    
    return 0;
}

