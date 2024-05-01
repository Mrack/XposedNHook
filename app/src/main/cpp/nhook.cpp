//
// Created by Mrack on 2024/4/19.
//

#include "nhook.h"

#include "demo/qbdihook.h"
#include "demo/ytbssl.h"
#include "demo/modmenu_native.h"
#include "linker_hook.h"
#include <unistd.h>
#include <dlfcn.h>
#include "demo/il2cpp_dumper.h"

extern "C"
{

JNIEXPORT void JNICALL
Java_cn_mrack_xposed_nhook_NHook_initNativeHook(JNIEnv *env, jclass thiz, jobject context) {
    LOGD("initNativeHook");
    gContext = env->NewGlobalRef(context);
    LOGD("test_QBDI");
    test_QBDI();
    LOGD("test_youtube");
    test_youtube();
    LOGD("dump_il2cpp");
    dumpIL2cpp(get_data_path(gContext));
}


JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGD("JNI_OnLoad");
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    gVm = vm;
    return JNI_VERSION_1_6;
}
}