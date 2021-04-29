#pragma once
#include "netscenecustom.h"

class NetSceneVirusUser {
  public:
    static const char *const    kOkResp;
    static const char *const    kWrongParamsResp;
    static const char *const    kUnknownErrResp;
    
    class NetSceneVirusLogin : public NetSceneCustom {
      public:
        NetSceneVirusLogin();
        
        int GetType() override;
        
        int DoSceneImpl(const std::string &_in_buffer) override;
        
        NetSceneBase *NewInstance() override;
        
        void *Data() override;
        
        size_t Length() override;
        
        void CustomHttpHeaders(std::map<std::string, std::string> &_headers) override;
        
        const char *Route() override;
        
        const char *ContentType() override;
    
      private:
        static const char *const    kUrlRoute;
        static const char *const    kWrongPwdResp;
        char *                      resp_;
    };
    
    class NetSceneVirusRegister : public NetSceneCustom {
      public:
        NetSceneVirusRegister();
        
        int GetType() override;
        
        void CustomHttpHeaders(std::map<std::string, std::string> &_headers) override;
        
        int DoSceneImpl(const std::string &_in_buffer) override;
        
        NetSceneBase *NewInstance() override;
        
        void *Data() override;
        
        size_t Length() override;
        
        const char *Route() override;
        
        const char *ContentType() override;
    
      private:
        static const char * const   kUrlRoute;
        static const char *const    kEmailAlreadyRegistered;
        char *                      resp_;
    };
    
    class NetSceneVirusForgetPwd : public NetSceneCustom {
      public:
        
        NetSceneVirusForgetPwd();
        
        int GetType() override;
        
        void CustomHttpHeaders(std::map<std::string, std::string> &_headers) override;
        
        int DoSceneImpl(const std::string &_in_buffer) override;
        
        NetSceneBase *NewInstance() override;
        
        void *Data() override;
        
        size_t Length() override;
        
        const char *Route() override;
        
        const char *ContentType() override;
      private:
        static const char * const   kUrlRoute;
        static const char *const    kNotRegisteredYet;
        char *                      resp_;
    };
    


  private:
    static int __ResolveEmailPassword(const std::string &_http_body,
                                      std::string &_email,
                                      std::string &_pwd,
                                      char **_resp);
    static void __CustomHttpHeaders(std::map<std::string, std::string> &_headers);
    
};
