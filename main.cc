#include "log.h"
#include "httpserver.h"

int main(int argc, char **argv) {
    LogI("main", "Launching SVR...");
    
    HttpServer::Instance().Run();
    
    return 0;
}

