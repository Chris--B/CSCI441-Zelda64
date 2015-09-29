cmake_minimum_required(VERSION 2.8)
project(GuildWars)

set(S "${CMAKE_SOURCE_DIR}")
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${S}/cmake")

find_package(OpenGL)
find_package(GLUT)

find_package(OpenAL)
include(FindALUT)

# CMake must be manually updated anytime a new file is added, when using globs.
# You can
#       (1) update this file's timestamp, or
#       (2) re-run 'cmake' manually
file(GLOB_RECURSE sources "${S}/source/*.cpp")
file(GLOB_RECURSE includes "${S}/include/*.hpp")

include_directories("include")

if (CMAKE_COMPILER_IS_GNUCXX)
  # Warnings to turn ON
  set(Wwarnings "-Wall -Wextra")

  # Warnings to turn OFF
  set(Wno-warnings "-Wno-unused-parameter")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Wwarnings} ${Wno-warnings}")
endif()

add_executable(guildWars ${sources} ${includes})
target_link_libraries(guildWars
                      ${OPENGL_LIBRARIES}
                      ${GLUT_LIBRARIES}
                      ${OPENAL_LIBRARIES}
                      ${ALUT_LIBRARIES}
                      )