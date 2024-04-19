//
// Created by Mrack on 2024/4/19.
//

#ifndef NHOOK_H
#define NHOOK_H

#include <string>
#include <jni.h>
#include "linker_hook.h"
#include <android/log.h>

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "MainHook", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "MainHook", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "MainHook", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, "MainHook", __VA_ARGS__)

void module_load(const char *filename);

#endif