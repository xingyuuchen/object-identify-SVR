#include "registry.h"
#include "netscenedispatcher.h"

#include "netscene_queryimg/netscene_queryimg.h"
#include "netscene_covid19infected/netscene_getcovid19infected.h"
#include "netscene_gettrainprogress.h"
#include "netscene_getrecentquery.h"
#include "netscene_gethotsearch.h"
#include "netscene_register.h"
#include "netscene_uploadavatar.h"
#include "netscene_changenickname.h"


namespace Registry {

void RegisterNetScenes() {
    NetSceneDispatcher::Instance().RegisterNetScene(new NetSceneQueryImg());
    NetSceneDispatcher::Instance().RegisterNetScene(new NetSceneGetTrainProgress());
    NetSceneDispatcher::Instance().RegisterNetScene(new NetSceneRegister());
    NetSceneDispatcher::Instance().RegisterNetScene(new NetSceneUploadAvatar());
    NetSceneDispatcher::Instance().RegisterNetScene(new NetSceneGetHotSearch());
    NetSceneDispatcher::Instance().RegisterNetScene(new NetSceneGetRecentQuery());
    NetSceneDispatcher::Instance().RegisterNetScene(new NetSceneChangeNickname());
    NetSceneDispatcher::Instance().RegisterNetScene(new NetSceneGetCovid19Infected());
    
}


}