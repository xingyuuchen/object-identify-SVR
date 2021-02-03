#ifndef OI_SVR_LOG_H
#define OI_SVR_LOG_H

#include "stdio.h"
#include "time/timeutil.h"

#define LogI(...) PrintCurrTime(); \
    printf(" I/"); \
    printf(__VA_ARGS__); \
    printf("\n");


#define LogE(...) PrintCurrTime(); \
    printf(" E/"); \
    printf(__VA_ARGS__); \
    printf("\n");


#endif //OI_SVR_LOG_H
