//
// Created by Mrack on 2024/4/19.
//

#ifndef XPOSEDNHOOK_LINKER_HOOK_H
#define XPOSEDNHOOK_LINKER_HOOK_H

#include "utils.h"
#include "dobby/dobby.h"
#include <set>

void hook_module_load();
void *get_address_from_module(const char *module_path, const char *symbol_name);

#endif //XPOSEDNHOOK_LINKER_HOOK_H
