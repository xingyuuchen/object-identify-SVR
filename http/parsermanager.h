#ifndef OI_SVR_PARSERMANAGER_H
#define OI_SVR_PARSERMANAGER_H

#include <unordered_map>
#include <memory>
#include "httprequest.h"
#include "../socket/unix_socket.h"


namespace http { namespace request {

class ParserManager {
  public:
    static ParserManager &Instance() {
        static ParserManager instance;
        return instance;
    }
    void operator=(ParserManager const &) = delete;
    ParserManager(ParserManager const &) = delete;
    
    int DeleteParser(SOCKET _fd);
    
    int ContainsParser(SOCKET _fd);
    
    std::shared_ptr<http::request::Parser> GetParser(SOCKET _fd);
    
  private:
    ParserManager();
    
    std::shared_ptr<http::request::Parser> __CreateParser(SOCKET _fd);

  private:
    std::unordered_map<SOCKET, std::shared_ptr<http::request::Parser>> parsers_map_;
    
};

}}



#endif //OI_SVR_PARSERMANAGER_H
