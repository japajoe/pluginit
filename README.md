# pluginit
A small library for loading shared libraries at runtime. This can be used for hot reloading plugins.

# Example
To dynamically load a plugin you'll need a host application and a shared library. A shared library can either be an .so on Linux or a .dll on Windows. First create a host application that will load the plugin:
```c
#include "pluginit.h"

typedef void (*entry_point_ptr)(void);

int main(int argc, char **argv) {
    //Loads a plugin by given filepath
    void *pluginHandle = pli_plugin_load("./libsomelib.so");
    
    //Exit if plugin failed to load
    if(pluginHandle == NULL)
        return 1;

    //Finds a method in the plugin by its symbol name
    void *pEntryPoint = pli_plugin_get_symbol(pluginHandle, "plugin_entry_point");

    //If no symbol was found, unload the plugin and exit
    if(pEntryPoint == NULL) {
        pli_plugin_unload(pluginHandle);
        return 1;
    }

    //Symbol was found, cast the pointer to the method to the appropriate function pointer type
    entry_point_ptr entryPoint = (entry_point_ptr)pEntryPoint;

    //Call the method in the plugin
    entryPoint();

    //Unload the plugin
    pli_plugin_unload(pluginHandle);

    return 0;
}
```
Here is an example of a plugin (shared library) that can be loaded by the host application:
```c
#include <stdio.h>

#define PLUGIN_EXPORT extern

#if defined(__cplusplus)
extern "C" {
#endif

PLUGIN_EXPORT void plugin_entry_point(void) {
    printf("Plugin says hello\n");
}

#if defined(__cplusplus)
}
#endif
```
For a more elaborate demo see the [C++ example](https://github.com/japajoe/pluginit/tree/main/example).

# Building
To make a build you shouldn't have to link anything on Windows. On Linux you only need to link with `-ldl`. Building for Mac is currently not supported but I'm happy to accept pull requests for this.
