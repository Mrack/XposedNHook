//
// Created by Mrack on 2024/4/30.
//

#ifndef XPOSEDNHOOK_IL2CPP_DUMPER_H
#define XPOSEDNHOOK_IL2CPP_DUMPER_H

#include <stddef.h>
#include <string>
#include <map>
#include <dlfcn.h>
#include <assert.h>
#include "il2cpp-tabledefs.h"
#include "il2cpp-class.h"

bool dump(int, const char *);

void dumpIL2cpp(const char *);

void *get_il2cpp_handle();


class Il2CppApi {
public:
    static std::map<std::string, void *> il2cpp_functions;

    static void *get(const std::string &name) {
        if (il2cpp_functions.find(name) == il2cpp_functions.end()) {
            void *address = dlsym(il2cpp_handle, name.c_str());
            if (address == nullptr) {
                return nullptr;
            }
            il2cpp_functions[name] = address;
        }
        assert(il2cpp_functions[name] != nullptr);
        return il2cpp_functions[name];
    }

    static void *il2cpp_handle;
    static size_t il2cpp_base;
};

#define IL2CPP_API(functionName, returnType, paramType, param) \
    typedef returnType (*functionName##_t) paramType;          \
    static functionName##_t functionName##_ptr;                \
    static returnType functionName paramType { \
        functionName##_ptr = (functionName##_t)Il2CppApi::get(#functionName); \
        return functionName##_ptr param; \
    }

#endif //XPOSEDNHOOK_IL2CPP_DUMPER_H
