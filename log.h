#ifndef OI_SVR_LOG_H
#define OI_SVR_LOG_H

#include "stdio.h"

#define Log(...)    printf(__VA_ARGS__); \
    printf("\n");

#endif //OI_SVR_LOG_H
