#include "utils/log.h"
#include "httpserver.h"


int main(int argc, char **argv) {
    LogI("Launching SVR...");
    
    HttpServer::Instance().Run();
    
    return 0;
}

