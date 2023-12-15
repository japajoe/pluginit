#include "Plugin.hpp"
#include "pluginit.h"

Plugin::Plugin(const std::string &filePath, proc_address_func procAddressFunc) {
    this->handle = nullptr;
    this->filePath = filePath;
    this->procAddressFunc = procAddressFunc;
    this->onLoad = nullptr;
    this->onUnload = nullptr;
}

bool Plugin::Load() {
    if(handle)
        return false;

    handle = pli_plugin_load(filePath.c_str());
    
    if(!handle) 
        return false;

    void *pOnLoad = FindSymbol("OnPluginLoad");

    if(!pOnLoad) {
        pli_plugin_unload(handle);
        onLoad = nullptr;
        onUnload = nullptr;
        return false;
    }
    
    void *pOnUnload = FindSymbol("OnPluginUnload");

    if(!pOnUnload) {
        pli_plugin_unload(handle);
        onLoad = nullptr;
        onUnload = nullptr;
        return false;
    }

    onLoad = reinterpret_cast<plugin_load>(pOnLoad);
    onUnload = reinterpret_cast<plugin_unload>(pOnUnload);

    onLoad(procAddressFunc);

    return true;
}

bool Plugin::Unload() {
    if(!handle)
        return false;

    if(!onUnload) 
        return false;

    onUnload();

    pli_plugin_unload(handle);

    handle = nullptr;

    return true;
}

void *Plugin::FindSymbol(const std::string &symbolName) {
    return pli_plugin_get_symbol(handle, symbolName.c_str());
}
