# ArcVideoFoundation - Core library for ArcVideo
# Copyright (C) 2023 Olive Studios LLC
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

project(arcvideo-foundation VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Compiler options ---
if(MSVC)
  set(FOUNDATION_COMPILE_OPTIONS
    /wd4267
    /wd4244
    /experimental:external
    /external:anglebrackets
    /external:W0
    "$<$<CONFIG:RELEASE>:/O2>"
    "$<$<COMPILE_LANGUAGE:CXX>:/MP>"
  )
else()
  set(FOUNDATION_COMPILE_OPTIONS
    "$<$<CONFIG:RELEASE>:-O2>"
    -Wuninitialized
    -pedantic-errors
    -Wall
    -Wextra
    -Wno-unused-parameter
    -Wshadow
  )
endif()

# --- Dependencies ---
# Only when building standalone (not as subdirectory of ArcVideo)
if(PROJECT_IS_TOP_LEVEL)
  # OpenEXR / Imath (vcpkg CONFIG)
  find_package(Imath CONFIG REQUIRED)

  # FFmpeg (vcpkg)
  if(DEFINED VCPKG_INSTALLED_DIR)
    include("${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/ffmpeg/FindFFMPEG.cmake")
    if(NOT FFMPEG_FOUND)
      message(FATAL_ERROR "FFmpeg not found via vcpkg")
    endif()
    if(NOT TARGET FFMPEG::FFMPEG)
      add_library(FFMPEG::FFMPEG IMPORTED INTERFACE)
      target_include_directories(FFMPEG::FFMPEG INTERFACE ${FFMPEG_INCLUDE_DIRS})
      target_link_libraries(FFMPEG::FFMPEG INTERFACE ${FFMPEG_LIBRARIES})
    endif()
  endif()
endif()

# OpenTime (optional, from parent project or vcpkg)
find_package(opentime CONFIG QUIET)

if(TARGET OTIO::opentime)
  set(FOUNDATION_HAS_OPENTIME TRUE)
elseif(TARGET opentime)
  set(FOUNDATION_HAS_OPENTIME TRUE)
endif()

# --- Gather sources via GLOB ---
file(GLOB_RECURSE FOUNDATION_HEADERS
  "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

file(GLOB_RECURSE FOUNDATION_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# --- Library target ---
add_library(arcvideo-foundation STATIC
  ${FOUNDATION_HEADERS}
  ${FOUNDATION_SOURCES}
)

target_include_directories(arcvideo-foundation
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_options(arcvideo-foundation PRIVATE ${FOUNDATION_COMPILE_OPTIONS})

# Link dependencies
target_link_libraries(arcvideo-foundation
  PUBLIC
    Imath::Imath
    FFMPEG::FFMPEG
)

if(FOUNDATION_HAS_OPENTIME)
  if(TARGET OTIO::opentime)
    target_link_libraries(arcvideo-foundation PUBLIC OTIO::opentime)
  else()
    target_link_libraries(arcvideo-foundation PUBLIC opentime)
  endif()
  target_compile_definitions(arcvideo-foundation PUBLIC USE_OTIO)
endif()

# Platform definitions
if(WIN32)
  target_compile_definitions(arcvideo-foundation PUBLIC UNICODE _UNICODE NOMINMAX)
endif()

# --- Alias for parent project consumption ---
add_library(ArcVideo::Foundation ALIAS arcvideo-foundation)
