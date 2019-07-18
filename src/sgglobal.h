#ifndef SGGLOBAL_H
#define SGGLOBAL_H


#ifdef BUILDING_PLUGIN
    #ifdef _MSC_VER
        #define INTER_LIB_API __declspec(dllexport)
    #else
        #define INTER_LIB_API __attribute__((visibility("default")))
    #endif
#else
    #ifdef _MSC_VER
        #define INTER_LIB_API __declspec(dllimport)
    #else
        #define INTER_LIB_API __attribute__((visibility("default")))
    #endif
#endif

#endif // SGGLOBAL_H
