#include "pluginit.h"
#include "stdio.h"

#if defined(_WIN32)
    #define PLATFORM_WINDOWS
#endif
#if defined(__linux__) || defined(__FreeBSD__)
    #define PLATFORM_LINUX
#endif

#if defined (PLATFORM_WINDOWS)
    #include <windows.h>
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
    s = (void*)GetProcAddress((HINSTANCE)pluginHandle, symbolName);
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

char *pli_find_library_path(const char *libraryName) {
    static char result[4096]; // Static buffer to hold result
#ifdef PLATFORM_WINDOWS
    DWORD result = SearchPath(NULL, libraryName, NULL, MAX_PATH, result, NULL);
    if (result == 0) {
        // No match found, handle the error
        return NULL;
    }
    int len = strlen(result);
    char *outputPath = (char*)malloc(len+1);
    memcpy(outputPath, result, len);
    outputPath[len] = '\0';
    return outputPath; // Return the path found
#endif

#ifdef PLATFORM_LINUX
    // Prepare the command to search the library
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "ldconfig -p 2>/dev/null | grep %s", libraryName);
    
    FILE* pipe = popen(cmd, "r");
    if (!pipe) {
        perror("popen() failed");
        return NULL;
    }

    while (fgets(result, sizeof(result), pipe) != NULL) {
        // Find the path after the "=>" symbol
        char* pos = strstr(result, "=>");
        if (pos != NULL) {
            pos += 2; // Move pointer to the path
            // Trim whitespace
            while (*pos == ' ') pos++;
            // Remove newline character
            char* newline = strchr(pos, '\n');
            if (newline) *newline = '\0';
            pclose(pipe);
            int len = strlen(pos);
            char *outputPath = (char*)malloc(len+1);
            memcpy(outputPath, pos, len);
            outputPath[len] = '\0';
            return outputPath; // Return the path found
        }
    }
    
    pclose(pipe);
    return NULL; // Return NULL if not found
#endif
}

void pli_free_library_path(char *libraryPath) {
    if(libraryPath)
        free(libraryPath);
}
