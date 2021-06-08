#include "netscene_virususer.h"
#include "../netscenetypes.h"
#include "log.h"
#include "strutil.h"
#include "dbitem_virususer.h"
#include "dao/connection.h"


const char *const NetSceneVirusUser::NetSceneVirusRegister::kUrlRoute = "/virus/register";
const char *const NetSceneVirusUser::NetSceneVirusLogin::kUrlRoute = "/virus/login";
const char *const NetSceneVirusUser::NetSceneVirusForgetPwd::kUrlRoute = "/virus/forget-password";

const char *const NetSceneVirusUser::kOkResp = R"({"errcode":0, "errmsg":"success"})";
const char *const NetSceneVirusUser::kWrongParamsResp = R"({"errcode":-1, "errmsg":"Wrong request params"})";
const char *const NetSceneVirusUser::kUnknownErrResp = R"({"errcode":-1, "errmsg":"Unknown err"})";
const char *const NetSceneVirusUser::NetSceneVirusLogin::kWrongPwdResp = R"({"errcode":-1, "errmsg":"Wrong email or password"})";
const char *const NetSceneVirusUser::NetSceneVirusForgetPwd::kNotRegisteredYet = R"({"errcode":-1, "errmsg":"This email not registered yet"})";
const char *const NetSceneVirusUser::NetSceneVirusRegister::kEmailAlreadyRegistered = R"({"errcode":-1, "errmsg":"This email already registered"})";


NetSceneVirusUser::NetSceneVirusLogin::NetSceneVirusLogin()
        : NetSceneCustom()
        , resp_(nullptr) {
}

int NetSceneVirusUser::NetSceneVirusLogin::GetType() { return kNetSceneTypeVirusLogin; }

NetSceneBase *NetSceneVirusUser::NetSceneVirusLogin::NewInstance() { return new NetSceneVirusLogin(); }

void *NetSceneVirusUser::NetSceneVirusLogin::Data() { return (char *) resp_; }

size_t NetSceneVirusUser::NetSceneVirusLogin::Length() { return strlen(resp_); }

const char *NetSceneVirusUser::NetSceneVirusLogin::Route() { return kUrlRoute; }

const char *NetSceneVirusUser::NetSceneVirusLogin::ContentType() {
    return http::HeaderField::kApplicationJson;
}

void NetSceneVirusUser::NetSceneVirusLogin::CustomHttpHeaders(std::map<std::string, std::string> &_headers) {
    __CustomHttpHeaders(_headers);
}

int NetSceneVirusUser::NetSceneVirusLogin::DoSceneImpl(const std::string &_in_buffer) {
    LogI("%s", _in_buffer.c_str())
    std::string email;
    std::string password;
    
    if (__ResolveEmailPassword(_in_buffer, email, password, &resp_) < 0) {
        return 0;
    }
    
    DBItem_VirusUser user;
    user.SetEmail(email);
    user.SetUserPwd(password);
    bool exist = false;
    if (dao::QueryExist(user, exist) < 0) {
        LogE("db query failed")
        resp_ = (char *) kUnknownErrResp;
        return -1;
    }
    if (!exist) {
        LogI("wrong user_name or pwd")
        resp_ = (char *) kWrongPwdResp;
        return 0;
    }
    resp_ = (char *) kOkResp;
    return 0;
}




NetSceneVirusUser::NetSceneVirusRegister::NetSceneVirusRegister()
        : NetSceneCustom()
        , resp_(nullptr) { }

int NetSceneVirusUser::NetSceneVirusRegister::GetType() { return kNetSceneTypeVirusRegister; }

void NetSceneVirusUser::NetSceneVirusRegister::CustomHttpHeaders(std::map<std::string, std::string> &_headers) {
    __CustomHttpHeaders(_headers);
}

NetSceneBase *NetSceneVirusUser::NetSceneVirusRegister::NewInstance() {
    return new NetSceneVirusUser::NetSceneVirusRegister();
}

void *NetSceneVirusUser::NetSceneVirusRegister::Data() { return (char *) resp_; }

size_t NetSceneVirusUser::NetSceneVirusRegister::Length() { return strlen(resp_); }

const char *NetSceneVirusUser::NetSceneVirusRegister::Route() { return kUrlRoute; }

const char *NetSceneVirusUser::NetSceneVirusRegister::ContentType() { return http::HeaderField::kApplicationJson; }

int NetSceneVirusUser::NetSceneVirusRegister::DoSceneImpl(const std::string &_in_buffer) {
    LogI("%s", _in_buffer.c_str())
    std::string email;
    std::string password;
    
    if (__ResolveEmailPassword(_in_buffer, email, password, &resp_) < 0) {
        return 0;
    }
    DBItem_VirusUser neo;
    neo.SetEmail(email);
    bool registered = false;
    if (dao::QueryExist(neo, registered)) {
        LogE("db QueryExist failed")
        resp_ = (char *) kUnknownErrResp;
        return -1;
    }
    if (registered) {
        LogI("%s already registered", email.c_str())
        resp_ = (char *) kEmailAlreadyRegistered;
        return 0;
    }
    neo.SetUserPwd(password);
    
    if (dao::Insert(neo) < 0) {
        LogE("db insert failed")
        resp_ = (char *) kUnknownErrResp;
        return 0;
    }
    resp_ = (char *) kOkResp;
    return 0;
}


NetSceneVirusUser::NetSceneVirusForgetPwd::NetSceneVirusForgetPwd()
        : NetSceneCustom()
        , resp_(nullptr) {
}

int NetSceneVirusUser::NetSceneVirusForgetPwd::GetType() { return kNetSceneTypeVirusForgerPwd; }

void NetSceneVirusUser::NetSceneVirusForgetPwd::CustomHttpHeaders(std::map<std::string, std::string> &_headers) {
    __CustomHttpHeaders(_headers);
}

int NetSceneVirusUser::NetSceneVirusForgetPwd::DoSceneImpl(const std::string &_in_buffer) {
    LogI("%s", _in_buffer.c_str())
    std::string email;
    std::string password;
    
    if (__ResolveEmailPassword(_in_buffer, email, password, &resp_) < 0) {
        return 0;
    }
    
    DBItem_VirusUser old;
    old.SetEmail(email);
    bool registered = false;
    if (dao::QueryExist(old, registered)) {
        LogE("db QueryExist failed")
        resp_ = (char *) kUnknownErrResp;
        return -1;
    }
    if (!registered) {
        resp_ = (char *) kNotRegisteredYet;
        return 0;
    }
    DBItem_VirusUser neo;
    neo.SetUserPwd(password);
    if (dao::Update(old, neo) < 0) {
        LogE("db update failed")
        resp_ = (char *) kUnknownErrResp;
        return 0;
    }
    resp_ = (char *) kOkResp;
    return 0;
}

NetSceneBase *NetSceneVirusUser::NetSceneVirusForgetPwd::NewInstance() {
    return new NetSceneVirusUser::NetSceneVirusForgetPwd();
}

void *NetSceneVirusUser::NetSceneVirusForgetPwd::Data() { return (char *) resp_; }

size_t NetSceneVirusUser::NetSceneVirusForgetPwd::Length() { return strlen(resp_); }

const char *NetSceneVirusUser::NetSceneVirusForgetPwd::Route() { return kUrlRoute; }

const char *NetSceneVirusUser::NetSceneVirusForgetPwd::ContentType() { return http::HeaderField::kApplicationJson; }


void NetSceneVirusUser::__CustomHttpHeaders(std::map<std::string, std::string> &_headers) {
    _headers[http::HeaderField::kAccessControlAllowOrigin] = http::HeaderField::kAccessControlOriginAll;
}

int NetSceneVirusUser::__ResolveEmailPassword(const std::string &_http_body,
                                              std::string &_email, std::string &_pwd,
                                              char **_resp) {
    std::vector<std::string> params;
    str::split(_http_body, "&", params);
    
    for (auto &param : params) {
        std::vector<std::string> pair;
        str::split(param, "=", pair);
        if (pair.size() != 2) {
            LogI("pair: %s", param.c_str())
            *_resp = (char *) kWrongParamsResp;
            return -1;
        }
        if (pair[0] == "email") {
            _email = pair[1];
        } else if (pair[0] == "password") {
            _pwd = pair[1];
        }
    }
    if (_email.empty() || _email.empty()) {
        LogI("no email or password")
        *_resp = (char *) kWrongParamsResp;
        return -1;
    }
    return 0;
}
