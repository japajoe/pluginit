#include "pluginit.h"
#include "stdio.h"

#if defined(_WIN32)
    #define PLATFORM_WINDOWS
#endif
#if defined(__linux__) || defined(__FreeBSD__)
    #define PLATFORM_LINUX
#endif

#if defined (PLATFORM_WINDOWS)
    #include <Windows.h>
#endif
#if defined (PLATFORM_LINUX)
    #include <dlfcn.h>
#endif

static pli_bool pli_file_exists(const char *filePath) {
    FILE *file;
    if((file = fopen(filePath, "r")) != NULL) {
        fclose(file);
        return PLI_TRUE;
    }
    return PLI_FALSE;
}

void *pli_plugin_load(const char *filePath) {
    if(!pli_file_exists) {
        printf("File not found: %s\n", filePath);
        return NULL;
    }

    void *moduleHandle = NULL;

#if defined (PLATFORM_WINDOWS)
    moduleHandle = (void*)LoadLibrary(filePath);
    if(!moduleHandle) {        
        printf("Failed to load plugin: %s\n", filePath);
    }    
#endif
    
#if defined (PLATFORM_LINUX)
    moduleHandle = dlopen(filePath, RTLD_LAZY);
    if(!moduleHandle) {
        char *error = dlerror();
        printf("Failed to load plugin: %s. Error %s\n", filePath, error);
    }
#endif

    return moduleHandle;
}

void pli_plugin_unload(void *pluginHandle) {
#if defined (PLATFORM_WINDOWS)
    if(pluginHandle != NULL) {
        FreeLibrary((HINSTANCE)pluginHandle);
    }
#endif

#if defined (PLATFORM_LINUX)
    if(pluginHandle != NULL) {
        dlclose(pluginHandle);
    }
#endif
}

void *pli_plugin_get_symbol(void *pluginHandle, const char *symbolName) {
    if(!pluginHandle) {
        return NULL;
    }

    void *s = NULL;

#if defined (PLATFORM_WINDOWS)
    s = GetProcAddress((HINSTANCE)pluginHandle, symbolName);
    if(s == NULL) {
        printf("Error: undefined symbol: %s\n", symbolName);
    }
#endif

#if defined (PLATFORM_LINUX)
    dlerror(); /* clear error code */
    s = dlsym(pluginHandle, symbolName);
    char *error = dlerror();

    if (error != NULL) {
        printf("Error: %s\n", error);
    }
#endif

    return s;
}