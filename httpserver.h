#ifndef OI_SVR_HTTPSERVER_H
#define OI_SVR_HTTPSERVER_H
#include <stdint.h>


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
    
  private:
    static const int    kBuffSize;
    bool                running_;
    int                 listenfd_;
    
};


#endif //OI_SVR_HTTPSERVER_H
