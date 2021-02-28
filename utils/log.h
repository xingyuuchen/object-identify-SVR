#ifndef OI_SVR_LOG_H
#define OI_SVR_LOG_H

#include "stdio.h"
#include "timeutil.h"

#define LogI(TAG, ...) PrintCurrTime(); \
    printf(" I/%s: ", TAG); \
    printf(__VA_ARGS__); \
    printf("\n");


#define LogE(TAG, ...) PrintCurrTime(); \
    printf(" E/%s: ", TAG); \
    printf(__VA_ARGS__); \
    printf("\n");


#endif //OI_SVR_LOG_H
