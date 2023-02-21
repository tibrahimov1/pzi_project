#pragma once

#ifdef MU_WINDOWS
    #ifdef VND_LIB_EXPORTS
        #define VND_LIB_API __declspec(dllexport)
    #else
        #define VND_LIB_API __declspec(dllimport)
    #endif
#else
    #ifdef VND_LIB_EXPORTS
        #define VND_LIB_API __attribute__((visibility("default")))
    #else
        #define VND_LIB_API
    #endif
#endif

