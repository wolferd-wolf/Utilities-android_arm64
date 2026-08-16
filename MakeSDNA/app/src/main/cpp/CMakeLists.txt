# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("makesdna")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             native-lib.cpp )

# Standalone CLI executable, added 2026-08-15: the Android emulator path
# (KVM/root/system-image ABI matching) turned out to be a dead end in
# GH-hosted CI for reasons that have nothing to do with this tool itself
# (see stage1-setup.yml job2 comments for the full story). native-lib.cpp
# is a trivial JNI shim calling a plain C main() that already lives inside
# libmakesdna.a (this IS Blender's own standalone makesdna CLI tool) - so
# build that main() as an actual standalone arm64-v8a executable instead
# and run it directly under qemu-user-static. cli_main.c is an
# intentionally empty translation unit; it exists only so add_executable
# has a source file to compile, the real main() gets pulled from the
# imported static lib below to satisfy the CRT's undefined reference.
add_executable( makesdna_cli cli_main.c )

set(blenderLibDir    D:/FreeProjects/Blender/Blender/Blender-android_arm64/)
set(blenderLibLibDir D:/FreeProjects/Blender/Blender/Utilities-android_arm64/MakeBlender/app/.cxx/cmake/release/arm64-v8a/lib/)

target_include_directories(native-lib PUBLIC
        ${blenderLibDir}/source/blender/makesdna
        ${blenderLibDir}/source/blender/makesrna
        )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

SET(BlenderLibNames
        makesdna
        )

# bf_bmesh依赖bf_blenkernel
# bf_blenkernel依赖bf_bmesh
# 其它不同模块也或多或少相互依赖
set(BlenderLibs
        ${BlenderLibNames}
        )
foreach (blenderLib ${BlenderLibs})
    add_library(${blenderLib} STATIC IMPORTED)
    set_target_properties(${blenderLib} PROPERTIES IMPORTED_LOCATION ${blenderLibLibDir}/lib${blenderLib}.a)
endforeach ()

target_link_libraries( # Specifies the target library.
        native-lib
        -Wl,--start-group
        ${BlenderLibNames}
        -Wl,--end-group
        )

target_link_libraries( makesdna_cli
        -Wl,--start-group
        ${BlenderLibNames}
        -Wl,--end-group
        )
set_target_properties( makesdna_cli PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/cli_output"
        LINK_FLAGS "-static"
        LINKER_LANGUAGE CXX )
# 2026-08-16: LINKER_LANGUAGE CXX added after run 31936460696's real link
# error - cli_main.c being a plain .c file made CMake pick the C driver
# (clang.exe) to link, which doesn't pull in the C++ runtime (libc++,
# exceptions, RTTI, __cxa_*, __gxx_personality_v0) that libmakesdna.a's
# C++ object code (e.g. guardedalloc's memory_usage.cc) needs. Forcing
# CXX makes CMake use clang++ for the final link step instead, matching
# what already happens automatically for native-lib.cpp next door.
# 2026-08-15 (second pass): briefly tried dynamic linking to match
# native-lib, matching what the real working OBlender app does - reverted
# again. qemu-user-static needs to actually execute the guest's dynamic
# linker (/system/bin/linker64 on Android) under emulation for a
# dynamically-linked binary; that binary is part of the Android OS image,
# not something the NDK ships, and there's no real/emulated device in this
# environment to source it from. Static avoids needing an interpreter at
# all. The __android_log_* symbols this used to get from ${log-lib} now
# come from cli_main.c's own stub implementations instead (NDK doesn't
# ship a redistributable static liblog.a for general linking anyway).

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-lib

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )