//
// Created by Mrack on 2024/4/20.
//

#include "qbdihook.h"
#include <jni.h>
#include <cstring>
#include <cstdio>

#include <fstream>
#include "vm.h"
#include "utils.h"

void vm_handle_add(void *address, DobbyRegisterContext *ctx) {
    LOGT("vm address %p ", address);
    DobbyDestroy(address);
    auto vm_ = new vm();
    auto qvm = vm_->init(address);
    auto state = qvm.getGPRState();
    syn_regs(ctx, state);
    uint8_t *fakestack;
    QBDI::allocateVirtualStack(state, 0x800000, &fakestack);
    qvm.call(nullptr, (uint64_t) address);
    QBDI::alignedFree(fakestack);

    // write to file
    std::ofstream out;
    std::string data = get_data_path(gContext);
    out.open(data + "/trace_log.txt", std::ios::out);
    out << vm_->logbuf.str();
    out.close();
}


void test_QBDI() {
    DobbyInstrument((void *) (Java_cn_mrack_xposed_nhook_NHook_sign1), vm_handle_add);
}


unsigned char s[256];
unsigned char t[256];


void swap(unsigned char *p1, unsigned char *p2) {
    unsigned char t = *p1;
    *p1 = *p2;
    *p2 = t;
}

void rc4_init(unsigned char *key, int key_len) {
    int i, j = 0;

    //Initial values of both vectors
    for (i = 0; i < 256; i++) {
        s[i] = i;
        t[i] = key[i % key_len];
    }
    //Initial permutation
    for (i = 0; i < 256; i++) {
        j = (j + s[i] + t[i]) % 256;
        swap(&s[i], &s[j]);
    }
}

void rc4(unsigned char *key, int key_len, char *buff, int len) {
    int i = 0;
    unsigned long t1, t2;
    unsigned char val;
    unsigned char out;
    t1 = 0;
    t2 = 0;
    rc4_init(key, key_len);

    //process one byte at a time
    for (i = 0; i < len; i++) {
        t1 = (t1 + 1) % 256;
        t2 = (t2 + s[t1]) % 256;
        swap(&s[t1], &s[t2]);
        val = (s[t1] + s[t2]) % 256;
        out = *buff ^ val;
        *buff = out;
        buff++;
    }
}

extern "C" JNIEXPORT jstring JNICALL
Java_cn_mrack_xposed_nhook_NHook_sign1(JNIEnv *env, jclass thiz, jstring sign) {
    const char *sign_ = env->GetStringUTFChars(sign, 0);
    char *res_chars = new char[strlen(sign_)];
    strcpy(res_chars, sign_);
    auto *key = (u_char *) "\x01\x02\x03\x04\x05";
    rc4(key, sizeof(key), res_chars, strlen(sign_));
    char *hex = new char[strlen(sign_) * 2 + 1];
    for (int i = 0; i < strlen(sign_); i++) {
        sprintf(hex + i * 2, "%02x", res_chars[i]);
    }
    env->ReleaseStringUTFChars(sign, sign_);
    return env->NewStringUTF(hex);
}

