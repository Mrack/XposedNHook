//
// Created by Mrack on 2024/4/19.
//

#ifndef XPOSEDNHOOK_UTILS_H
#define XPOSEDNHOOK_UTILS_H

#include <string>
#include "dobby/dobby.h"
#include <assert.h>
#include <android/log.h>
#include <jni.h>

#ifdef NDEBUG
#define LOGD(...)
#define LOGE(...)
#define LOGI(...)
#define LOGW(...)
#else
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "MainHook", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "MainHook", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "MainHook", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, "MainHook", __VA_ARGS__)
#endif
#define ANDROID_O 26
#define ANDROID_O2 27
#define ANDROID_P 28
#define ANDROID_Q 29
#define ANDROID_R 30
#define ANDROID_S 31
static int SDK_INT = -1;


extern JavaVM *gVm;

extern jobject gContext;

const char *get_data_path(jobject context);

int get_sdk_level();

char *get_linker_path();

std::pair<size_t, size_t> find_info_from_maps(const char *soname);

const char *find_path_from_maps(const char *soname);

int boyer_moore_search(u_char *haystack, size_t haystackLen, u_char *needle, size_t needleLen);

int search_hex(u_char *haystack, size_t haystackLen, const char *needle);

uint64_t get_arg(DobbyRegisterContext *ctx, int index);

void *get_address_from_module(const char *module_path, const char *symbol_name);

#endif //XPOSEDNHOOK_UTILS_H
