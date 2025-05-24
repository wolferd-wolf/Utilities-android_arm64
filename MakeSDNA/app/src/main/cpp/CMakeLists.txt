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