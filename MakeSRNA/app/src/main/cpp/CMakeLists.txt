# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("makesrna")

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

# Standalone CLI executable, added 2026-08-15 - see MakeSDNA/CMakeLists.txt
# for the full rationale. cli_main.c is intentionally empty; main() comes
# from the imported static libs below (this IS Blender's own standalone
# makesrna CLI tool).
add_executable( makesrna_cli cli_main.c )

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
        bf_intern_ghost
        bf_gpu
        bf_blenfont
        bf_blenlib
        bf_nodes_function
        bf_functions
        bf_python_gpu
        bf_intern_mantaflow
        bf_python_ext
        bf_avi
        bf_ikplugin
        bf_imbuf_openexr
        bf_editor_space_userpref
        bf_imbuf_openimageio
        bf_intern_opencolorio
        bf_editor_lattice
        bf_editor_mask
        bf_imbuf_cineon
        bf_intern_memutil
        bf_editor_space_script
        bf_editor_metaball
        bf_dna
        bf_compositor_shaders
        bf_dna_blenlib
        bf_gpu_shaders
        bf_draw_shaders
        bf_editor_datafiles
        bf_intern_opensubdiv
        bf_ocio_shaders
        extern_mantaflow
        bf_intern_quadriflow
        extern_quadriflow
        bf_intern_libmv
        bf_intern_audaspace
        audaspace-py
        extern_ceres
        audaspace
        extern_glog
        extern_gflags
        cycles_bvh
        cycles_scene
        cycles_integrator
        cycles_session
        cycles_device
        bf_intern_rigidbody
        extern_lzma
        extern_bullet
        extern_clew
        extern_minilzo
        bf_intern_iksolver
        bf_intern_itasc
        bf_intern_eigen
        bf_intern_dualcon
        bf_intern_openvdb
        bf_intern_locale
        bf_intern_sky
        cycles_kernel
        cycles_graph
        cycles_util
        extern_curve_fit_nd
        extern_rangetree
        extern_wcwidth
        cycles_subd
        bf_intern_libc_compat
        bf_intern_clog
        bf_intern_guardedalloc
        makesrna
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

target_link_libraries( makesrna_cli
        -Wl,--start-group
        ${BlenderLibNames}
        -Wl,--end-group
        )
set_target_properties( makesrna_cli PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/cli_output"
        LINK_FLAGS "-static"
        LINKER_LANGUAGE CXX )
# 2026-08-16: LINKER_LANGUAGE CXX added after run 31936460696's real link
# error - see MakeSDNA/CMakeLists.txt for the full rationale, same fix
# applies here (cli_main.c being plain C made CMake pick the C link
# driver, missing the C++ runtime this huge BlenderLibNames set needs).
# 2026-08-15 (second pass): briefly tried dynamic linking to match
# native-lib/the real OBlender app - reverted again. qemu-user-static
# needs to actually execute the guest's dynamic linker
# (/system/bin/linker64 on Android) under emulation for a dynamically
# linked binary; that's part of the Android OS image, not something the
# NDK ships, and there's no real/emulated device in this environment to
# source it from. Static avoids needing an interpreter at all. The
# __android_log_* symbols now come from cli_main.c's own stub
# implementations (NDK doesn't ship a redistributable static liblog.a for
# general linking). Deliberately NOT linking GLESv3/GLESv2/EGL/android/
# jnigraphics here even though native-lib does - unlike logging, which
# Blender code calls into pervasively, GPU/graphics functions are only
# actually referenced by bf_gpu's rendering code paths, which makesrna's
# headless RNA-generation main() should never reach. If the static link
# fails on one of those symbols, that's real evidence of exactly which
# ones are needed and worth stubbing - guessing the full GLES/EGL surface
# blindly isn't worth the risk of stubbing something wrong.
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