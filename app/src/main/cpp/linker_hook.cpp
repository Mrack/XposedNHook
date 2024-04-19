//
// Created by Mrack on 2024/4/19.
//

#include "linker_hook.h"
#include "elfio/elfio.hpp"
#include "nhook.h"

void *get_address_from_module(const char *module_path, const char *symbol_name) {
    ELFIO::elfio elffile;
    const char *file_name = strrchr(module_path, '/');
    elffile.load(module_path);
    size_t module_base = find_info_from_maps(file_name).first;
    ELFIO::symbol_section_accessor symbol_accessor(elffile, elffile.sections[".dynsym"]);
    for (int i = 0; i < symbol_accessor.get_symbols_num(); ++i) {
        std::string name;
        ELFIO::Elf64_Addr value;
        ELFIO::Elf_Xword size;
        unsigned char bind;
        unsigned char type;
        ELFIO::Elf_Half section_index;
        unsigned char other;
        symbol_accessor.get_symbol(i, name, value, size, bind, type, section_index, other);
        if (name.find(symbol_name) != std::string::npos) {
            return (void *) (value + module_base);
        }
    }
    return nullptr;
}

void *(*android_dlopen_ext)(const char *filename, int flags, const void *extinfo);

void *hook_android_dlopen_ext(const char *filename, int flags, const void *extinfo) {
    void *ret = android_dlopen_ext(filename, flags, extinfo);
    module_load(filename);
    return ret;
}

void hook_module_load() {
    void * address = get_address_from_module(get_linker_path(), "android_dlopen_ext");
    if (address != nullptr) {
        DobbyHook(address, (void *) hook_android_dlopen_ext, (void **) &android_dlopen_ext);
    }
}