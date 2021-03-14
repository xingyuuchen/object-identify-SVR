#include "log.h"
#include "httpserver.h"

int main(int ac, char **argv) {
    Logger::OpenLog(argv[0]);

    LogI("main", "Launching SVR...")
    
    HttpServer::Instance().Run();
    
    return 0;
}

