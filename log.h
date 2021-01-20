#ifndef OI_SVR_LOG_H
#define OI_SVR_LOG_H

#include "stdio.h"

#define LogI(...)    printf(__VA_ARGS__); \
    printf("\n");

// TODO LogE

#define LogE(...)    printf(__VA_ARGS__); \
    printf("\n");


#endif //OI_SVR_LOG_H
