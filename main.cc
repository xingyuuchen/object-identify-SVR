#include <signal.h>
#include "utils/log.h"
#include "httpserver.h"

void Stop(int _sig) {
    LogI("signal: %d, process Exit", _sig)
    if (_sig == 2) {
        HttpServer::Instance().Stop();
    }
}


int main(int argc, char **argv) {
    LogI(" Hello World...");
    signal(2, Stop);
    
    HttpServer::Instance().Run();
    return 0;
}

