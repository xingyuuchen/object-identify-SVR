#ifndef OI_SVR_LOG_H
#define OI_SVR_LOG_H

#include "stdio.h"

#define LogI(...)    printf("I/"); \
    printf(__VA_ARGS__); \
    printf("\n");


#define LogE(...)    printf("E/"); \
    printf(__VA_ARGS__); \
    printf("\n");


#endif //OI_SVR_LOG_H
