#include <unordered_map>
#include <iostream>
#include "Plugin.hpp"

std::unordered_map<std::string,void*> procMap;

static void *GetProcAddressFromHost(const char *procName) {
    if(procMap.count(procName) > 0) 
        return procMap[procName];
    return nullptr;
}

static void TestMethod(const char *message) {
    std::cout << message << '\n';
}

int main(int argc, char **argv) {
    procMap["TestMethod"] = reinterpret_cast<void*>(&TestMethod);

    proc_address_func procAddressFunc = reinterpret_cast<proc_address_func>(&GetProcAddressFromHost);

    //Rename file name to whatever you called your plugin
    Plugin plugin("./libtestplugin.so", procAddressFunc);

    if(!plugin.Load())
        return 1;

    if(!plugin.Unload())
        return 1;

    return 0;
}