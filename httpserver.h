#ifndef OI_SVR_HTTPSERVER_H
#define OI_SVR_HTTPSERVER_H


class HttpServer {
  public:
    static HttpServer &Instance() {
        static HttpServer instance;
        return instance;
    }
    
    void Run();
    
    void Stop();
    
    void operator=(const HttpServer &) = delete;
    
    HttpServer(const HttpServer &) = delete;

  private:
    HttpServer();
    
    void __Exit();
    
  private:
    static const int    kBuffSize;
    bool                running_;
    int                 listenfd_;
    
};


#endif //OI_SVR_HTTPSERVER_H
