#include "netscene_bearshopregister.h"
#include "../netscenetypes.h"
#include "fileutil.h"
#include "strutil.h"
#include "log.h"
#include "dao/connection.h"
#include "dbitem_bearuser.h"



const char *const NetSceneBearShopRegister::kUrlRoute = "/bear-shop-register";

const char *const NetSceneBearShopRegister::kRedirectToBearShopResp =
        R"(<head><meta http-equiv="refresh" content="0; URL=http://49.235.29.121:5002/file/xjx/bear_shop.html" /></head>)";

const char *const NetSceneBearShopRegister::kUsernameAlreadyUsedResp = "Your username already been used!!";

const char *const NetSceneBearShopRegister::kRegisterFailedResp =
        "Register Failed! Contact Stacy Xu for help.";


NetSceneBearShopRegister::NetSceneBearShopRegister()
        : NetSceneCustom()
        , resp_(nullptr) {
    resp_ = (char *) kRegisterFailedResp;
}

void *NetSceneBearShopRegister::Data() {
    return resp_;
}

size_t NetSceneBearShopRegister::Length() {
    return strlen(resp_);
}

int NetSceneBearShopRegister::GetType() { return kNetSceneTypeBearShopRegister; }

const char *NetSceneBearShopRegister::Route() { return kUrlRoute; }

NetSceneBase *NetSceneBearShopRegister::NewInstance() { return new NetSceneBearShopRegister(); }

const char *NetSceneBearShopRegister::GetBearShopRedirectResp() { return kRedirectToBearShopResp; }

const char *NetSceneBearShopRegister::ContentType() {
    return http::HeaderField::kTextHtml;
}

int NetSceneBearShopRegister::DoSceneImpl(const std::string &_in_buffer) {
    std::string::size_type params_start = _in_buffer.find('?');
    if (params_start == std::string::npos) {
        LogI("no ? found")
        return -1;
    }
    // Do Register
    std::string user_name;
    std::string user_pwd;
    
    std::string params_str = _in_buffer.substr(params_start + 1);
    std::vector<std::string> params;
    str::split(params_str, "&", params);
    for (auto &param : params) {
        std::vector<std::string> pair;
        str::split(param, "=", pair);
        if (pair.size() != 2) {
            LogE("pair: %s", param.c_str())
            return -1;
        }
        if (pair[0] == "user_name") {
            user_name = pair[1];
        } else if (pair[0] == "pwd") {
            user_pwd = pair[1];
        }
    }
    
    if (user_name.empty() || user_pwd.empty()) {
        LogE("no user-name or user-pwd")
        return -1;
    }
    
    DBItem_BearUser user;
    user.SetUserName(user_name);
    bool exist = false;
    if (Dao::QueryExist(user, exist) < 0) {
        LogE("db query failed, ")
        return -1;
    }
    if (exist) {
        resp_ = (char *) kUsernameAlreadyUsedResp;
        return -1;
    }
    
    user.SetUserPwd(user_pwd);
    
    int db_ret = Dao::Insert(user);
    if (db_ret < 0) {
        LogE("db insert failed")
        return -1;
    }
    
    resp_ = (char *) kRedirectToBearShopResp;
    return 0;
}

