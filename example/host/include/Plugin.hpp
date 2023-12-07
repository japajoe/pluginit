#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <string>

typedef void *(*proc_address_func)(const char*);
typedef void (*plugin_load)(proc_address_func);
typedef void (*plugin_unload)(void);

class Plugin {
private:
    void *handle;
    std::string filePath;
    proc_address_func procAddressFunc;
    plugin_load onLoad;
    plugin_unload onUnload;
public:
    Plugin(const std::string &filePath, proc_address_func procAddressFunc);
    bool Load();
    bool Unload();
    void *FindSymbol(const std::string &symbolName);
};

#endif