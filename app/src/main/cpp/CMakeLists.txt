# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible
# with ${CMAKE_PROJECT_NAME} (both CMake variables are in-sync within the top level
# build script scope).
project("nhook")

set(CMAKE_CXX_STANDARD 20)


set(LINKER_FLAGS "-ffixed-x18 -Wl,--hash-style=both")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=format -fdata-sections -ffunction-sections -fvisibility=hidden -Wl,--exclude-libs,ALL")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti -fvisibility=hidden -Wl,--exclude-libs,ALL")

include_directories(.)

find_library(egl EGL)
find_library(GLESV2_LIB GLESv2)
find_library(android android)
add_library(local_dobby STATIC IMPORTED)
add_library(local_qbdi STATIC IMPORTED)

set_target_properties(local_dobby PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_ABI}/libdobby.a)
set_target_properties(local_qbdi PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_ABI}/libQBDI.a)

include_directories(imgui)

add_library(${CMAKE_PROJECT_NAME} SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        nhook.cpp
        linker_hook.cpp
        vm.cpp
        utils.cpp

        #demo
        demo/qbdihook.cpp
        demo/modmenu.cpp
        demo/ytbssl.cpp
        demo/modmenu_native.cpp
        demo/il2cpp_dumper.cpp
)



include_directories(./imgui)

add_library(imgui STATIC
        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/backends/imgui_impl_android.cpp
        imgui/backends/imgui_impl_opengl3.cpp)

# Specifies libraries CMake should link to your target library. You
# can link libraries from various origins, such as libraries defined in this
# build script, prebuilt third-party libraries, or Android system libraries.
target_link_libraries(${CMAKE_PROJECT_NAME}
        # List libraries link to the target library
        android
        local_dobby
        local_qbdi
        EGL
        GLESv3
        imgui
        log)