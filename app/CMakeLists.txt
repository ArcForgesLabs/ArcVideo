# ArcVideo - Non-Linear Video Editor
# Copyright (C) 2022 ArcVideo Team
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

# Auto-discover all C++ source files
file(GLOB_RECURSE ARCVIDEO_SOURCES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

# Exclude files belonging to other targets
list(FILTER ARCVIDEO_SOURCES EXCLUDE REGEX ".*/main\\.cpp$")
list(FILTER ARCVIDEO_SOURCES EXCLUDE REGEX ".*/version\\.(cpp|h)$")
list(FILTER ARCVIDEO_SOURCES EXCLUDE REGEX ".*/crashhandler/.*")
list(FILTER ARCVIDEO_SOURCES EXCLUDE REGEX ".*/core/.*")

# Resource generation subdirectories (these generate .qrc files)
add_subdirectory(shaders)
add_subdirectory(render/ocioconf)
add_subdirectory(ui/style/arcvideo-dark)
add_subdirectory(ui/style/arcvideo-light)

# Static resource references
set(ARCVIDEO_RESOURCES
  ${ARCVIDEO_RESOURCES}
  ui/cursors/cursors.qrc
  ui/graphics/graphics.qrc
)

# Translations
add_subdirectory(ts)
qt_add_translation(ARCVIDEO_QM_FILES ${ARCVIDEO_TS_FILES})

set(QRC_BODY "")
foreach(QM_FILE ${ARCVIDEO_QM_FILES})
  get_filename_component(QM_FILENAME_COMPONENT ${QM_FILE} NAME_WE)
  string(APPEND QRC_BODY "<file alias=\"${QM_FILENAME_COMPONENT}\">${QM_FILE}</file>\n")
endforeach()
configure_file(ts/translations.qrc.in ts/translations.qrc @ONLY)

set(ARCVIDEO_RESOURCES
  ${ARCVIDEO_RESOURCES}
  ${CMAKE_CURRENT_BINARY_DIR}/ts/translations.qrc
)

# Add version object
add_library(arcvideo-version-obj
  OBJECT
  version.cpp
  version.h
)
target_link_libraries(arcvideo-version-obj PRIVATE Qt6::Core)
target_compile_options(arcvideo-version-obj PRIVATE -DAPPVERSION="${PROJECT_VERSION}" -DAPPVERSIONLONG="${PROJECT_LONG_VERSION}" )

# Add main library
add_library(libarcvideo-editor
  OBJECT
  ${ARCVIDEO_SOURCES}
)

# Remove prefix - prevents CMake calling it "liblibarcvideo-editor"
set_target_properties(libarcvideo-editor PROPERTIES PREFIX "")

# Add application
add_executable(arcvideo-editor
  main.cpp
  $<TARGET_OBJECTS:libarcvideo-editor>
  $<TARGET_OBJECTS:arcvideo-version-obj>
  ${ARCVIDEO_RESOURCES}
)

# Create docs if doxygen was found
if(DOXYGEN_FOUND)
  set(DOXYGEN_PROJECT_NAME "ArcVideo")
  set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/docs")
  set(DOXYGEN_EXTRACT_ALL "YES")
  set(DOXYGEN_EXTRACT_PRIVATE "YES")
  doxygen_add_docs(docs ALL ${ARCVIDEO_SOURCES})
endif()

# Platform-specific deployment preferences
if (WIN32)
  # Set Windows application icon
  target_sources(arcvideo-editor PRIVATE packaging/windows/resources.rc)

  # Preserve folder structure in visual studio
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ARCVIDEO_SOURCES})

elseif(APPLE)
  # Set Mac application icon
  set(ARCVIDEO_ICON packaging/macos/arcvideo.icns)
  target_sources(arcvideo-editor PRIVATE ${ARCVIDEO_ICON})

  # Set Mac bundle properties
  set_target_properties(arcvideo-editor PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/packaging/macos/MacOSXBundleInfo.plist.in
    MACOSX_BUNDLE_GUI_IDENTIFIER org.arcvideoeditor.ArcVideo
    MACOSX_BUNDLE_ICON_FILE arcvideo.icns
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
    MACOSX_BUNDLE_BUNDLE_NAME "ArcVideo"
    MACOSX_BUNDLE_INFO_STRING "ArcVideo ${PROJECT_LONG_VERSION}"
    MACOSX_BUNDLE_COPYRIGHT "©2018-2021 ArcVideo Studios LLC and others. Published under the GNU General Public License version 3.0."
    RESOURCE "${ARCVIDEO_ICON}"
    OUTPUT_NAME "ArcVideo"
  )
elseif(UNIX)
  # Set Linux-specific properties for application
  install(TARGETS arcvideo-editor RUNTIME DESTINATION bin)
endif()

# Set link libraries
target_link_libraries(arcvideo-editor PRIVATE ${ARCVIDEO_LIBRARIES})
target_link_libraries(libarcvideo-editor PRIVATE ${ARCVIDEO_LIBRARIES})

# Set compile options
target_compile_options(arcvideo-editor PRIVATE ${ARCVIDEO_COMPILE_OPTIONS})
target_compile_options(libarcvideo-editor PRIVATE ${ARCVIDEO_COMPILE_OPTIONS})

# Set global definitions
target_compile_definitions(arcvideo-editor PRIVATE ${ARCVIDEO_DEFINITIONS})
target_compile_definitions(libarcvideo-editor PRIVATE ${ARCVIDEO_DEFINITIONS})

# Set include dirs
target_include_directories(arcvideo-editor PRIVATE ${ARCVIDEO_INCLUDE_DIRS})
target_include_directories(libarcvideo-editor PRIVATE ${ARCVIDEO_INCLUDE_DIRS})

# Add crash handler
if (crashpad_FOUND AND Qt6Network_FOUND)
  add_subdirectory(crashhandler)
endif()

# Platform packaging
add_subdirectory(packaging)
