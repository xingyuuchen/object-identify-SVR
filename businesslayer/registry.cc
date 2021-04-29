#include "registry.h"
#include "netscenedispatcher.h"

#include "netscene_queryimg/netscene_queryimg.h"
#include "covid19infected/netscene_getcovid19infected.h"
#include "covid19infected/netscene_virususer.h"
#include "bear_shop/netscene_bearshopregister.h"
#include "bear_shop/netscene_bearshoplogin.h"
#include "netscene_gettrainprogress.h"
#include "netscene_getrecentquery.h"
#include "netscene_gethotsearch.h"
#include "netscene_register.h"
#include "netscene_uploadavatar.h"
#include "netscene_changenickname.h"
#include "netscene_getfile.h"


namespace registry {

void RegisterNetScenes() {
    NetSceneDispatcher::Instance().RegisterNetScene<NetSceneQueryImg>();
    NetSceneDispatcher::Instance().RegisterNetScene<NetSceneGetTrainProgress>();
    NetSceneDispatcher::Instance().RegisterNetScene<NetSceneRegister>();
    NetSceneDispatcher::Instance().RegisterNetScene<NetSceneUploadAvatar>();
    NetSceneDispatcher::Instance().RegisterNetScene<NetSceneGetHotSearch>();
    NetSceneDispatcher::Instance().RegisterNetScene<NetSceneGetRecentQuery>();
    NetSceneDispatcher::Instance().RegisterNetScene<NetSceneChangeNickname>();
    NetSceneDispatcher::Instance().RegisterNetScene<NetSceneGetCovid19Infected>();
    NetSceneDispatcher::Instance().RegisterNetScene<NetSceneGetFile>();
    NetSceneDispatcher::Instance().RegisterNetScene<NetSceneBearShopRegister>();
    NetSceneDispatcher::Instance().RegisterNetScene<NetSceneBearShopLogin>();
    NetSceneDispatcher::Instance().RegisterNetScene<NetSceneVirusUser::NetSceneVirusLogin>();
    NetSceneDispatcher::Instance().RegisterNetScene<NetSceneVirusUser::NetSceneVirusRegister>();
    NetSceneDispatcher::Instance().RegisterNetScene<NetSceneVirusUser::NetSceneVirusForgetPwd>();
    
}


}