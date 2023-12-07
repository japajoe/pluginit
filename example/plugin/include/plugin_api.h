#ifndef PLUGIN_API_H
#define PLUGIN_API_H

#if defined(_WIN32)
    #define PLUGIN_DLL_IMPORT  __declspec(dllimport)
    #define PLUGIN_DLL_EXPORT  __declspec(dllexport)
    #define PLUGIN_DLL_PRIVATE static
#else
    #if defined(__GNUC__) && __GNUC__ >= 4
        #define PLUGIN_DLL_IMPORT  __attribute__((visibility("default")))
        #define PLUGIN_DLL_EXPORT  __attribute__((visibility("default")))
        #define PLUGIN_DLL_PRIVATE __attribute__((visibility("hidden")))
    #else
        #define PLUGIN_DLL_IMPORT
        #define PLUGIN_DLL_EXPORT
        #define PLUGIN_DLL_PRIVATE static
    #endif
#endif

typedef void *(*proc_address_func)(const char*);
typedef void (*test_func)(const char *);

#if defined(__cplusplus)
extern "C" {
#endif

PLUGIN_DLL_EXPORT void OnPluginLoad(proc_address_func getProcAddress);
PLUGIN_DLL_EXPORT void OnPluginUnload(void);

#if defined(__cplusplus)
}
#endif

#endif