//
// Created by Mrack on 2024/4/28.
//

#ifndef XPOSEDNHOOK_QBDIHOOK_H
#define XPOSEDNHOOK_QBDIHOOK_H

#include "jni.h"
#include "dobby/dobby.h"

void test_QBDI();

extern "C" JNIEXPORT jstring JNICALL
Java_cn_mrack_xposed_nhook_NHook_sign1(JNIEnv *env, jclass thiz, jstring sign);
void vm_handle_add(void *address, DobbyRegisterContext *ctx);
#endif //XPOSEDNHOOK_QBDIHOOK_H
