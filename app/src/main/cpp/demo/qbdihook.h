//
// Created by Mrack on 2024/4/28.
//

#ifndef XPOSEDNHOOK_QBDIHOOK_H
#define XPOSEDNHOOK_QBDIHOOK_H

#include "jni.h"

void test_QBDI();

extern "C" JNIEXPORT jstring JNICALL
Java_cn_mrack_xposed_nhook_NHook_sign1(JNIEnv *env, jclass thiz, jstring sign);

#endif //XPOSEDNHOOK_QBDIHOOK_H
