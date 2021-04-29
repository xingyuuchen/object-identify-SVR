#ifndef OI_SVR_NETSCENETYPES_H
#define OI_SVR_NETSCENETYPES_H

#include "constantsprotocol.h"


/**
 * All NetScene types are declared here.
 */
const int kNetSceneTypeQueryImg             = kReservedTypeOffset + 1;
const int kNetSceneTypeGetTrainProgress     = kReservedTypeOffset + 2;
const int kNetSceneTypeRegister             = kReservedTypeOffset + 3;
const int kNetSceneTypeUploadAvatar         = kReservedTypeOffset + 4;
const int kNetSceneTypeGetHotSearch         = kReservedTypeOffset + 5;
const int kNetSceneTypeGetRecentQuery       = kReservedTypeOffset + 6;
const int kNetSceneTypeChangeNickname       = kReservedTypeOffset + 7;
const int kNetSceneTypeGetCovid19Infected   = kReservedTypeOffset + 8;
const int kNetSceneTypeGetFile              = kReservedTypeOffset + 9;
const int kNetSceneTypeBearShopRegister     = kReservedTypeOffset + 10;
const int kNetSceneTypeBearShopLogin        = kReservedTypeOffset + 11;
const int kNetSceneTypeVirusLogin           = kReservedTypeOffset + 12;
const int kNetSceneTypeVirusRegister        = kReservedTypeOffset + 13;
const int kNetSceneTypeVirusForgerPwd       = kReservedTypeOffset + 14;


#endif //OI_SVR_NETSCENETYPES_H
