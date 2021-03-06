#include "netscene_bearshoplogin.h"
#include "netscene_bearshopregister.h"
#include "../netscenetypes.h"
#include "fileutil.h"
#include "dao/connection.h"
#include "strutil.h"
#include "dbitem_bearuser.h"
#include "log.h"
#include <vector>


const char *const NetSceneBearShopLogin::kUrlRoute = "/bear-shop-login";

const char *const NetSceneBearShopLogin::kLoginErrResp = "Login Err!";
const char *const NetSceneBearShopLogin::kWrongPwdResp =
        "Wrong user name or password!";

NetSceneBearShopLogin::NetSceneBearShopLogin()
        : NetSceneCustom()
        , resp_(nullptr) {
    resp_ = (char *) kLoginErrResp;
}

void *NetSceneBearShopLogin::Data() {
    return resp_;
}

size_t NetSceneBearShopLogin::Length() {
    return strlen(resp_);
}

int NetSceneBearShopLogin::GetType() { return kNetSceneTypeBearShopLogin; }

const char *NetSceneBearShopLogin::Route() { return kUrlRoute; }

NetSceneBase *NetSceneBearShopLogin::NewInstance() { return new NetSceneBearShopLogin(); }

const char *NetSceneBearShopLogin::ContentType() {
    return http::HeaderField::kTextHtml;
}

int NetSceneBearShopLogin::DoSceneImpl(const std::string &_in_buffer) {
    std::string::size_type params_start = _in_buffer.find('?');
    if (params_start == std::string::npos) {
        LogI("no ? found")
        return -1;
    }
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
    user.SetUserPwd(user_pwd);
    bool exist = false;
    if (dao::QueryExist(user, exist) < 0) {
        LogE("db query failed, ")
        return -1;
    }
    if (!exist) {
        LogI("wrong user_name or pwd")
        resp_ = (char *) kWrongPwdResp;
        return -1;
    }
    
    resp_ = (char *) NetSceneBearShopRegister::GetBearShopRedirectResp();
    return 0;
}
