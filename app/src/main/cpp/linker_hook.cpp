//
// Created by Mrack on 2024/4/19.
//

#include "linker_hook.h"
#include "elfio/elfio.hpp"
#include "nhook.h"


install_hook_name(android_dlopen_ext, void *, const char *filename, int flags,
                  const void *extinfo) {
    void *ret = orig_android_dlopen_ext(filename, flags, extinfo);
    module_load(filename);
    return ret;
}

void hook_module_load() {
    void *address = get_address_from_module(get_linker_path(), "android_dlopen_ext");
    if (address != nullptr) {
        install_hook_android_dlopen_ext(address);
    } else {
        LOGD("hook_module_load: android_dlopen_ext not found");
    }
}

