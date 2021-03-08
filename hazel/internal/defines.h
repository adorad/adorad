#pragma once 

#define null (void*)0

// bool is a basic type in C++ and not C
// We could just have used <stdbool.h> but I prefer this -- smaller binary, easily maintanable
#ifndef __cplusplus
    typedef unsigned char bool;
    static const bool false = 0;
    static const bool true = 1;
#endif 