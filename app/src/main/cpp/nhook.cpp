//
// Created by Mrack on 2024/4/19.
//

#include "nhook.h"

extern "C"
JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    return JNI_VERSION_1_6;
}


int (*SSL_callback)(void *ctx, void *out_alert);

int hook_SSL_callback(void *ctx, void *out_alert) {
    int res = SSL_callback(ctx, out_alert);
    return 0;
}


void (*SSL_CTX_set_custom_verify)(void *ctx, int mode, void *callback);

void hook_SSL_CTX_set_custom_verify(void *ctx, int mode, void *callback) {
    LOGD("callback %p", callback);
    SSL_CTX_set_custom_verify(ctx, mode, callback);
    DobbyHook(callback, (void *) hook_SSL_callback,
              (void **) &SSL_callback);
}


void module_load(const char *file_path) {
    LOGD("module_load: %s", file_path);
    std::string path = file_path;
    if (path.find("libcronet") == std::string::npos) {
        return;
    }
    const char *file_name = strrchr(file_path, '/');
    const std::pair<size_t, size_t> &info = find_info_from_maps(file_name + 1);
    u_char *base = (u_char *) info.first;
    size_t size = info.second;
    int offset = search_hex((u_char *) base, size,
                            "????01B9????00F9C0035FD6");
    if (offset > 0) {
        void *p_SSL_CTX_set_custom_verify = (void *) (offset + info.first);
        LOGD("SSL_CTX_set_custom_verify: %p", p_SSL_CTX_set_custom_verify);
        DobbyHook(p_SSL_CTX_set_custom_verify, (void *) hook_SSL_CTX_set_custom_verify,
                  (void **) &SSL_CTX_set_custom_verify);
    }
}


extern "C"
JNIEXPORT void JNICALL
Java_cn_mrack_xposed_nhook_NHook_initNativeHook(JNIEnv *env, jclass thiz) {
    hook_module_load();
}