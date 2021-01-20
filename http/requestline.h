#ifndef OI_SVR_REQUESTLINE_H
#define OI_SVR_REQUESTLINE_H

#include <string>
#include "../autobuffer.h"


namespace http {

class RequestLine {
  public:
    enum THttpMethod {
        kGET = 0,
        kPOST,   // only support post
        kDELETE,
        // ...
        kMethodMax,
    };
    static const char *const method2string[kMethodMax];
    
    enum THttpVersion {
        kHTTP_0_9 = 0,
        kHTTP_1_0,
        kHTTP_1_1,   // only support 1.1
        kHTTP_2_0,
        kVersionMax,
    };
    static const char *const version2string[kVersionMax];
    

  public:
    
    RequestLine();
    
    void SetUrl(const std::string &_url);
    
    void SetMethod(THttpMethod _method);
    
    void SetVersion(THttpVersion _version);
    
    void ToString(std::string &_target);
    
    void AppendToBuffer(AutoBuffer &_buffer);

  private:
    THttpMethod method_;
    std::string url_;
    THttpVersion version_;
    
};

}

#endif //OI_SVR_REQUESTLINE_H
