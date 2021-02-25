#ifndef OI_SVR_HTTPSERVER_H
#define OI_SVR_HTTPSERVER_H
#include <stdint.h>
#include "socket/unix_socket.h"


class HttpServer {
  public:
    void operator=(const HttpServer &) = delete;
    HttpServer(const HttpServer &) = delete;
    
    static HttpServer &Instance() {
        static HttpServer instance;
        return instance;
    }
    
    void Run(uint16_t _port = 5002);
    
    void Stop();
    

  private:
    HttpServer();
    
    int __CreateListenFd();
    
    int __Bind(uint16_t _port);
    
    int __HandleConnect();
    
    int __HandleRead(SOCKET _fd);
    int __HandleReadTest(SOCKET _fd);
    
  private:
    static const int    kBuffSize;
    bool                running_;
    int                 listenfd_;
    
};


#endif //OI_SVR_HTTPSERVER_H
