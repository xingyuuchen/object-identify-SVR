#include "netscene_bearshopregister.h"
#include "../netscenetypes.h"
#include "fileutil.h"
#include "strutil.h"
#include "log.h"
#include "dao/connection.h"
#include "dbitem_bearuser.h"



std::string NetSceneBearShopRegister::kBearShopHtmlPath =
        "/file/xjx/bear_shop.html";
const char *const NetSceneBearShopRegister::kUrlRoute = "/bear-shop-register";

const char *const NetSceneBearShopRegister::kRegisterFailedResp =
        "Register Failed! Contact Stacy Xu for help.";


NetSceneBearShopRegister::NetSceneBearShopRegister()
        : NetSceneBase()
        , register_success_(false) {
}

bool NetSceneBearShopRegister::IsUseProtobuf() { return false; }

void *NetSceneBearShopRegister::Data() {
    if (register_success_) {
        return (void *) net_scene_get_file_.Data();
    }
    return (void *) kRegisterFailedResp;
}

size_t NetSceneBearShopRegister::Length() {
    if (register_success_) {
        return net_scene_get_file_.Length();
    }
    return strlen(kRegisterFailedResp);
}

int NetSceneBearShopRegister::GetType() { return kNetSceneTypeBearShopRegister; }

char *NetSceneBearShopRegister::Route() { return const_cast<char *>(kUrlRoute); }

NetSceneBase *NetSceneBearShopRegister::NewInstance() { return new NetSceneBearShopRegister(); }

std::string &NetSceneBearShopRegister::GetBearShopHtmlPath() { return kBearShopHtmlPath; }

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
    user.SetUserPwd(user_pwd);
    int db_ret = Dao::Insert(user);
    if (db_ret < 0) {
        LogE("db insert failed, ")
        return -1;
    }
    
    net_scene_get_file_.DoScene(kBearShopHtmlPath);
    register_success_ = true;
    return 0;
}

