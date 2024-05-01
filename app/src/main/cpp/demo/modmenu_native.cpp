//
// Created by Mrack on 2024/4/28.
//

#include <android/native_window_jni.h>
#include <android/input.h>
#include <EGL/egl.h>
#include <unistd.h>
#include "modmenu_native.h"
#include "thread"
#include "linker_hook.h"
#include "utils.h"
#include "imgui.h"
#include <GLES3/gl3.h>
#include "backends/imgui_impl_android.h"
#include "backends/imgui_impl_opengl3.h"

static int g_GlHeight, g_GlWidth;
static bool g_IsSetup = false;
static bool g_Orientation = false;
static std::string g_IniFileName = "";

static EGLDisplay g_EglDisplay = EGL_NO_DISPLAY;
static EGLSurface g_EglSurface = EGL_NO_SURFACE;
static EGLContext g_EglContext = EGL_NO_CONTEXT;


char *get_input_path() {
    char *input;
#if defined(__aarch64__)
    input = "/system/lib64/libinput.so";
#else
    input = "/system/lib/libinput.so";
#endif
    return input;
}


char *get_EGL_path() {
    char *input;
#if defined(__aarch64__)
    input = "/system/lib64/libEGL.so";
#else
    input = "/system/lib/libEGL.so";
#endif
    return input;
}


void ImGui_ImplAndroid_NewFrame(int width, int height)
{
    ImGuiIO& io = ImGui::GetIO();

    // Setup display size (every frame to accommodate for window resizing)
    int32_t window_width = (int32_t)width;
    int32_t window_height = (int32_t)height;
    int display_width = window_width;
    int display_height = window_height;

    io.DisplaySize = ImVec2((float)window_width, (float)window_height);
    if (window_width > 0 && window_height > 0)
        io.DisplayFramebufferScale = ImVec2((float)display_width / window_width, (float)display_height / window_height);

}

static bool cb = false;

void draw() {
    ImGuiIO &io = ImGui::GetIO();
    if (g_EglDisplay == EGL_NO_DISPLAY) {
        return;
    }
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(g_GlWidth, g_GlHeight);
    ImGui::NewFrame();

    static float colorPickerValue = 0.1f;
    static float seekBarValue = 0.5f;
    static bool checkBoxValue = false;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(g_GlWidth, g_GlHeight));

    ImGui::Begin("Demo", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    ImGui::Text("Hello, world!");

    if (ImGui::Button("Button")) {
        // Code to execute when the button is clicked
    }
    ImGui::SliderFloat("SeekBar", &seekBarValue, 0.0f, 1.0f);

    ImGui::Checkbox("CheckBox", &checkBoxValue);

    ImGui::ColorPicker4("ColorPicker", &colorPickerValue);

    ImGui::End();

    ImGui::Render();
    glViewport(0, 0, (int) io.DisplaySize.x, (int) io.DisplaySize.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    eglSwapBuffers(g_EglDisplay, g_EglSurface);
}


install_hook_name(initializeMotionEvent, void, void *thiz, void *event, void *msg) {
    orig_initializeMotionEvent(thiz, event, msg);
    if (g_IsSetup) {
        ImGui_ImplAndroid_HandleInputEvent((AInputEvent *) thiz);
    }
}

//
//void SetupImGui() {
//    if (g_IsSetup) return;
//    auto ctx = ImGui::CreateContext();
//    if (!ctx) {
//        LOGI(("Failed to create context"));
//        return;
//    }
//
//    ImGuiIO &io = ImGui::GetIO();
//    io.DisplaySize = ImVec2((float) g_GlWidth * 3, (float) g_GlHeight * 3);
//    io.ConfigWindowsMoveFromTitleBarOnly = true;
//    io.IniFilename = nullptr;
//
//    // Setup Platform/Renderer backends
//    ImGui_ImplAndroid_Init(nullptr);
//    ImGui_ImplOpenGL3_Init("#version 300 es");
//    ImGui::StyleColorsLight();
//    ImFontConfig font_cfg;
//    font_cfg.SizePixels = 30.0f;
//    io.Fonts->AddFontDefault(&font_cfg);
//
//    ImGui::GetStyle().ScaleAllSizes(5);
//
//    g_IsSetup = true;
//    LOGI("setup done.");
//}


//void menu_inject() {
//    input_inject();
//    void *peglSwapBuffers = ((void *) get_address_from_module(
//            get_EGL_path(), "eglSwapBuffers"));
//
//    if (peglSwapBuffers != nullptr) {
//        DobbyInstrument(peglSwapBuffers, [](void *address, DobbyRegisterContext *ctx) {
//            Window::instance()->getViewPort();
//            g_GlWidth = Window::instance()->getWidth();
//            g_GlHeight = Window::instance()->getHeight();
//            SetupImGui();
//            if (g_IsSetup) {
//                draw();
//            }
//        });
//    }
//}
//
//void test_modmenu_native() {
//    std::thread t(menu_inject);
//    t.detach();
//}


void input_inject() {
    void *pinitializeMotionEvent = ((void *) get_address_from_module(
            get_input_path(),
            "initializeMotionEvent"));
    if (pinitializeMotionEvent != nullptr) {
        install_hook_initializeMotionEvent(pinitializeMotionEvent);
    }
}

void init(ANativeWindow *window) {
    if (g_IsSetup)
        return;
    input_inject();
    ANativeWindow_acquire(window);

    {
        g_EglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (g_EglDisplay == EGL_NO_DISPLAY)
            LOGD("%s", "eglGetDisplay(EGL_DEFAULT_DISPLAY) returned EGL_NO_DISPLAY");

        if (eglInitialize(g_EglDisplay, 0, 0) != EGL_TRUE)
            LOGD("%s", "eglInitialize() returned with an error");

        const EGLint egl_attributes[] = {EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_RED_SIZE, 8,
                                         EGL_DEPTH_SIZE, 24, EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                                         EGL_NONE};
        EGLint num_configs = 0;
        if (eglChooseConfig(g_EglDisplay, egl_attributes, nullptr, 0, &num_configs) != EGL_TRUE)
            LOGD("%s", "eglChooseConfig() returned with an error");
        if (num_configs == 0)
            LOGD("%s", "eglChooseConfig() returned 0 matching config");

        // Get the first matching config
        EGLConfig egl_config;
        eglChooseConfig(g_EglDisplay, egl_attributes, &egl_config, 1, &num_configs);
        EGLint egl_format;
        eglGetConfigAttrib(g_EglDisplay, egl_config, EGL_NATIVE_VISUAL_ID, &egl_format);
        ANativeWindow_setBuffersGeometry(window, 0, 0, egl_format);

        const EGLint egl_context_attributes[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
        g_EglContext = eglCreateContext(g_EglDisplay, egl_config, EGL_NO_CONTEXT,
                                        egl_context_attributes);

        if (g_EglContext == EGL_NO_CONTEXT)
            LOGD("%s", "eglCreateContext() returned EGL_NO_CONTEXT");

        g_EglSurface = eglCreateWindowSurface(g_EglDisplay, egl_config, window, nullptr);
        eglMakeCurrent(g_EglDisplay, g_EglSurface, g_EglSurface, g_EglContext);
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();


    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplAndroid_Init(window);
    ImGui_ImplOpenGL3_Init("#version 300 es");
    ImFontConfig font_cfg;
    font_cfg.SizePixels = 42.0f;
    io.Fonts->AddFontDefault(&font_cfg);
    ImGui::GetStyle().ScaleAllSizes(5.0f);

    g_IsSetup = true;
}

const int FRAME_PER_SECOND = 30;
std::atomic<bool> g_Rendering(false);


extern "C"
JNIEXPORT void JNICALL
Java_cn_mrack_xposed_nhook_menu_SurfaceImGUI_initImGUI(JNIEnv *env, jclass clazz, jobject surface) {
    g_Rendering = true;
    g_IsSetup = false;
    ANativeWindow *w = ANativeWindow_fromSurface(env, (jobject) surface);
    g_GlWidth = ANativeWindow_getWidth(w);
    g_GlHeight = ANativeWindow_getHeight(w);
    init(w);
    while (g_Rendering) {
        draw();
        usleep(1000 / FRAME_PER_SECOND);
    }
    ANativeWindow_release(w);
    ImGui_ImplAndroid_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
}


extern "C"
JNIEXPORT void JNICALL
Java_cn_mrack_xposed_nhook_menu_SurfaceImGUI_setOrientationImGUI(JNIEnv *env, jclass clazz,
                                                                 jint orientation) {
    // TODO
}

extern "C"
JNIEXPORT void JNICALL
Java_cn_mrack_xposed_nhook_menu_SurfaceImGUI_destroyImGUI(JNIEnv *env, jclass clazz) {
    g_Rendering = false;
}