//
// Created by Mrack on 2024/4/28.
//

#include "ytbssl.h"
#include "utils.h"
#include "linker_hook.h"

int (*SSL_callback)(void *ctx, void *out_alert);

int hook_SSL_callback(void *ctx, void *out_alert) {
    int res = SSL_callback(ctx, out_alert);
    return 0;
}

void (*SSL_CTX_set_custom_verify)(void *ctx, int mode, void *callback);

void hook_SSL_CTX_set_custom_verify(void *ctx, int mode, void *callback) {
    LOGD("SSL_CTX_set_custom_verify callback %p", callback);
    SSL_CTX_set_custom_verify(ctx, mode, callback);
    DobbyHook(callback, (void *) hook_SSL_callback,
              (void **) &SSL_callback);
}

void test_youtube() {
    hook_module_load();
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

