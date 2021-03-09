#include "utils/log.h"
#include "httpserver.h"

#include <list>
int main(int argc, char **argv) {
    LogI("main", "Launching SVR...");
    
    std::list<int> ints;
    std::list<int>::iterator a = ints.begin();
    
    HttpServer::Instance().Run();
    
    return 0;
}

