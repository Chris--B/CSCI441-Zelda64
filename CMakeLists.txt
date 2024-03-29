cmake_minimum_required(VERSION 2.8)

project(csci441-Graphics-Project)

set(binary "keyToTheKingdom")
set(S "${CMAKE_SOURCE_DIR}")
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${S}/cmake")

option(USE_XINPUT "Build with support for Xbox 360 controllers" OFF)
if("${USE_XINPUT}")
  add_definitions("-DUSE_XINPUT")
  set(XINPUT_LIBS "Xinput.lib")
else()
  set(XINPUT_LIBS "")
endif()

option(USE_CLANG_FORMAT
    "Use clang-format to automatically format code before building."
    ON)

# Some of our stuff is GCC version dependent.
execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
                OUTPUT_VARIABLE GCC_VERSION)

# OpenGL
find_package(OpenGL REQUIRED)

# GLUT
find_package(GLUT REQUIRED)
include_directories("${GLUT_INCLUDE_DIR}")

# SOIL
find_package(SOIL REQUIRED)
include_directories("${SOIL_INCLUDE_DIR}")

# GLEW
find_package(GLEW REQUIRED)
include_directories("${GLEW_INCLUDE_DIR}")

# FMOD
find_package(FMOD REQUIRED)
include_directories("${FMOD_INCLUDE_DIRS}")

## External libraries

# C style printf made with with C++ templates. Just a single header.
include_directories("${S}/ext/tinyformat-include")

# Dr. Paone's object loader class and company. We embed a custom CMake project.
include_directories("${S}/ext/modelLoader")
add_subdirectory("${S}/ext/modelLoader")

# CMake must be manually updated anytime a new file is added, when using globs.
# You can
#       (1) update this file's timestamp, or
#       (2) re-run 'cmake' manually

# We do it this way instead of recurisve globs to preserve the file structure in
# projects like Visual Studio.
file(GLOB sources              "${S}/source/*.cpp")
file(GLOB sources_cameras      "${S}/source/Cameras/*.cpp")
file(GLOB sources_utils        "${S}/source/Utils/*.cpp")
file(GLOB sources_worldobjects "${S}/source/WorldObjects/*.cpp")
file(GLOB sources_lighting     "${S}/source/WorldObjects/Lighting/*.cpp")

file(GLOB includes              "${S}/include/*.hpp")
file(GLOB includes_cameras      "${S}/include/Cameras/*.hpp")
file(GLOB includes_utils        "${S}/include/Utils/*.hpp")
file(GLOB includes_worldobjects "${S}/include/WorldObjects/*.hpp")
file(GLOB includes_lighting     "${S}/include/WorldObjects/Lighting/*.hpp")

list(APPEND sources ${sources_cameras})
list(APPEND sources ${sources_utils})
list(APPEND sources ${sources_worldobjects})
list(APPEND sources ${sources_lighting})

list(APPEND includes ${includes_cameras})
list(APPEND includes ${includes_utils})
list(APPEND includes ${includes_worldobjects})
list(APPEND includes ${includes_lighting})

source_group("Header Files\\Cameras"      FILES ${includes_cameras})
source_group("Header Files\\Utils"        FILES ${includes_utils})
source_group("Header Files\\WorldObjects" FILES ${includes_worldobjects})
source_group("Header Files\\WorldObjects\\Lighting" FILES ${includes_lighting})

source_group("Source Files\\Cameras"      FILES ${sources_cameras})
source_group("Source Files\\Utils"        FILES ${sources_utils})
source_group("Source Files\\WorldObjects" FILES ${sources_worldobjects})

include_directories("${S}/include")

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
    # Misc flags
    add_compile_options("-std=c++11")
    if (NOT CMAKE_COMPILER_IS_CLANGXX)
      # Clang doesn't support this.
      add_compile_options("-fmax-errors=5")
    endif()

    if (GCC_VERSION VERSION_GREATER 4.8)
        message("Using gcc version ${GCC_VERSION}")
        # This doesn't work on older versions of GCC. Instead, they error.
        add_compile_options("-fdiagnostics-color")
    endif()

    # Warnings to turn ON
    add_compile_options("-Wall")
    add_compile_options("-Wextra")

    # Warnings to turn OFF
    add_compile_options("-Wno-char-subscripts")
    add_compile_options("-Wno-deprecated-declarations")

# This *might* catch Clang, but pretend it only catches MSVC for now.
else()
    # MSVC ships with reasonable defaults for compile options.

    # MSVC provides safe versions of a ton of functions, and then warns if you don't
    # use them.
    # We're not using them, so disable the warnings.
    add_definitions("-D_CRT_SECURE_NO_WARNINGS")
endif()

# This is the main binary we'll eventually turn in. Most assignments have a
# required name, but that is chosen by the Makefile Dr. Paone runs.
# We can use anything for CMake, since only Chris uses it.
add_executable(${binary}
               ${sources}
               ${includes})

target_link_libraries(${binary}
                      "${OPENGL_LIBRARIES}"
                      "${GLUT_LIBRARIES}"
                      "${GLEW_LIBRARIES}"
                      "${SOIL_LIBRARIES}"
                      "${FMOD_LIBRARIES}"
                      "${XINPUT_LIBS}"
                      # Dr Paone's wavfront model loader.
                      modelLoader)

# Move some stuff around so that Visual Studio debugging can be happy,
add_custom_target("Copy_Asset_Files" ALL
    COMMAND cmake -E copy_directory "${S}/assets/" "${CMAKE_BINARY_DIR}/assets/")
add_custom_target("Copy_Shaders" ALL
    COMMAND cmake -E copy_directory "${S}/glsl/" "${CMAKE_BINARY_DIR}/glsl/")

if("${USE_CLANG_FORMAT}")
    # TODO: Maybe use git-clang-format?
    # I'm not entirely sure what the difference is.
    add_custom_target(Format COMMAND clang-format ${sources} ${includes} -i)
    add_dependencies(${binary} Format)
endif()
