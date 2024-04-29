//
// Created by Mrack on 2024/4/23.
//
#include <jni.h>
#include <cstring>
#include <cstdio>

extern "C" JNIEXPORT jstring JNICALL
Java_cn_mrack_xposed_nhook_NHook_test111(JNIEnv *env, jclass thiz, jint value) {
    char buf[128];
    sprintf(buf, "value: %d", value);
    // write code here
    return env->NewStringUTF(buf);
}
