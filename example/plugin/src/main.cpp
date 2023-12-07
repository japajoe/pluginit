#include "plugin_api.h"
#include <iostream>

void OnPluginLoad(proc_address_func getProcAddress) {
    std::cout << "Plugin loaded\n";
    
    void * pTestMethod = getProcAddress("TestMethod");

    if(pTestMethod != nullptr) {
        test_func testMethod = reinterpret_cast<test_func>(pTestMethod);
        testMethod("Plugin says hello to host");
    }
}

void OnPluginUnload(void) {
    std::cout << "Plugin unloaded\n";
}