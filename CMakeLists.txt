# ArcVideo - Non-Linear Video Editor
# Copyright (C) 2023 ArcVideo Studios LLC
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.



cmake_minimum_required(VERSION 4.2 FATAL_ERROR)

project(arcvideo-editor VERSION 0.2.0 LANGUAGES CXX)

option(BUILD_DOXYGEN "Build Doxygen documentation" OFF)
option(BUILD_TESTS "Build unit tests" OFF)
option(USE_WERROR "Error on compile warning" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Output directories - exe/dll to <build-dir>/bin, libs to <build-dir>/lib
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Sanitizers
add_library(arcvideo-sanitizers INTERFACE)
include(cmake/Sanitizers.cmake)
enable_sanitizers(arcvideo-sanitizers)
list(APPEND ARCVIDEO_LIBRARIES arcvideo-sanitizers)

# Set compiler options
if(MSVC)
  set(ARCVIDEO_COMPILE_OPTIONS
    /wd4267
    /wd4244
    /experimental:external
    /external:anglebrackets
    /external:W0
    "$<$<CONFIG:RELEASE>:/O2>"
    "$<$<COMPILE_LANGUAGE:CXX>:/MP>"
  )
  if (USE_WERROR)
    list(APPEND ARCVIDEO_COMPILE_OPTIONS "/WX")
  endif()
else()
  set(ARCVIDEO_COMPILE_OPTIONS
    "$<$<CONFIG:RELEASE>:-O2>"
    -Wuninitialized
    -pedantic-errors
    -Wall
    -Wextra
    -Wno-unused-parameter
    -Wshadow
  )
  if (USE_WERROR)
    list(APPEND ARCVIDEO_COMPILE_OPTIONS "-Werror")
  endif()
endif()

set(ARCVIDEO_DEFINITIONS -DQT_DEPRECATED_WARNINGS)
if (WIN32)
  list(APPEND ARCVIDEO_DEFINITIONS -DUNICODE -D_UNICODE)
endif()

# --- OpenGL ---
if(UNIX AND NOT APPLE AND NOT DEFINED OpenGL_GL_PREFERENCE)
  set(OpenGL_GL_PREFERENCE LEGACY)
endif()
find_package(OpenGL REQUIRED)
list(APPEND ARCVIDEO_LIBRARIES OpenGL::GL)

# --- OpenColorIO (vcpkg CONFIG) ---
find_package(OpenColorIO CONFIG REQUIRED)
list(APPEND ARCVIDEO_LIBRARIES OpenColorIO::OpenColorIO)

# --- OpenImageIO (vcpkg CONFIG) ---
find_package(OpenImageIO CONFIG REQUIRED)
list(APPEND ARCVIDEO_LIBRARIES OpenImageIO::OpenImageIO OpenImageIO::OpenImageIO_Util)

# --- OpenEXR / Imath (vcpkg CONFIG) ---
find_package(OpenEXR CONFIG REQUIRED)
find_package(Imath CONFIG REQUIRED)
list(APPEND ARCVIDEO_LIBRARIES OpenEXR::OpenEXR Imath::Imath)

# --- Qt6 ---
set(QT_VERSION_MAJOR 6)
set(QT_LIBRARIES Core Gui Widgets OpenGL LinguistTools Concurrent)
if (UNIX AND NOT APPLE)
  list(APPEND QT_LIBRARIES DBus)
endif()
find_package(Qt6 REQUIRED COMPONENTS ${QT_LIBRARIES} OpenGLWidgets OPTIONAL_COMPONENTS Network)
if (NOT Qt6Network_FOUND)
  message(STATUS "Qt6::Network not found, crash reporting will be disabled.")
endif()
list(APPEND ARCVIDEO_LIBRARIES
  Qt6::Core Qt6::Gui Qt6::Widgets Qt6::OpenGL Qt6::Concurrent Qt6::OpenGLWidgets
)

# --- KDDockWidgets (submodule, built statically) ---
list(APPEND ARCVIDEO_LIBRARIES KDAB::kddockwidgets)

# --- FFmpeg (vcpkg) ---
# Directly include vcpkg's FindFFMPEG.cmake to avoid Qt6's bundled version
# (Qt6 adds its cmake/Qt6 dir to CMAKE_MODULE_PATH, which shadows vcpkg's find module)
# VCPKG_INSTALLED_DIR is set by vcpkg toolchain automatically
include("${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/ffmpeg/FindFFMPEG.cmake")
if(NOT FFMPEG_FOUND)
  message(FATAL_ERROR "FFmpeg not found via vcpkg")
endif()
if(NOT TARGET FFMPEG::FFMPEG)
  add_library(FFMPEG::FFMPEG IMPORTED INTERFACE)
  target_include_directories(FFMPEG::FFMPEG INTERFACE ${FFMPEG_INCLUDE_DIRS})
  target_link_libraries(FFMPEG::FFMPEG INTERFACE ${FFMPEG_LIBRARIES})
endif()
list(APPEND ARCVIDEO_LIBRARIES FFMPEG::FFMPEG)

# --- ArcVideoFoundation (pure C++ core library, no Qt) ---
add_subdirectory(foundation)
list(APPEND ARCVIDEO_LIBRARIES arcvideo-foundation)

# --- PortAudio (vcpkg CONFIG) ---
find_package(portaudio CONFIG REQUIRED)
list(APPEND ARCVIDEO_LIBRARIES portaudio)

# --- Optional: OpenTimelineIO ---
# Check if OTIO submodule is present (will be built in third_party/)
if (EXISTS "${CMAKE_SOURCE_DIR}/third_party/OpenTimelineIO/CMakeLists.txt")
  set(ARCVIDEO_HAVE_OTIO ON)
  list(APPEND ARCVIDEO_DEFINITIONS USE_OTIO)
  list(APPEND ARCVIDEO_LIBRARIES OTIO::opentimelineio)
  message(STATUS "OpenTimelineIO found (submodule), OTIO interchange enabled.")
else()
  set(ARCVIDEO_HAVE_OTIO OFF)
  message(STATUS "OpenTimelineIO not found, OTIO interchange will be disabled.")
endif()

# --- Optional: Crashpad (vcpkg) ---
find_package(crashpad CONFIG QUIET)
if (crashpad_FOUND)
  list(APPEND ARCVIDEO_DEFINITIONS USE_CRASHPAD)
  list(APPEND ARCVIDEO_LIBRARIES crashpad::crashpad)
  message(STATUS "Crashpad found, crash reporting enabled.")
else()
  message(STATUS "Crashpad not found, crash reporting will be disabled.")
  if (APPLE)
    list(APPEND ARCVIDEO_LIBRARIES "-framework ApplicationServices")
  endif()
endif()

# --- Platform-specific ---
if (APPLE)
  list(APPEND ARCVIDEO_LIBRARIES "-framework IOKit")
elseif(UNIX)
  list(APPEND ARCVIDEO_LIBRARIES Qt6::DBus)
endif()

# Generate Git hash
set(PROJECT_LONG_VERSION ${PROJECT_VERSION})
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
  find_package(Git)
  if(GIT_FOUND)
    execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short=8 HEAD
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE GIT_HASH
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    set(PROJECT_LONG_VERSION ${PROJECT_VERSION}-${GIT_HASH})
  endif()
endif()

# Optional: Find Doxygen if requested
if(BUILD_DOXYGEN)
  find_package(Doxygen)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
list(APPEND ARCVIDEO_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/third_party)

add_subdirectory(third_party)

# Propagate OTIO support to foundation (after third_party defines OTIO::opentime)
if(ARCVIDEO_HAVE_OTIO)
  target_compile_definitions(arcvideo-foundation PUBLIC USE_OTIO)
  target_link_libraries(arcvideo-foundation PUBLIC OTIO::opentime)
endif()

add_subdirectory(app)

if (BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

