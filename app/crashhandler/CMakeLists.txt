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

# Create crash handler executable
add_executable(
  arcvideo-crashhandler
  crashhandler.cpp
  crashhandler.h
  $<TARGET_OBJECTS:arcvideo-version-obj>
)

# Disable console appearing on crash handler dialog
set_target_properties(arcvideo-crashhandler PROPERTIES
  WIN32_EXECUTABLE TRUE
)

# Set crash handler includes
target_include_directories(
  arcvideo-crashhandler
  PRIVATE
  ${CMAKE_SOURCE_DIR}/app
)

# Set crash handler libs
target_link_libraries(
  arcvideo-crashhandler
  PRIVATE
  Qt6::Core
  Qt6::Gui
  Qt6::Widgets
  Qt6::Network
  crashpad::crashpad
)

set(CRASHPAD_HANDLER "crashpad_handler${CMAKE_EXECUTABLE_SUFFIX}")
set(MINIDUMP_STACKWALK "minidump_stackwalk${CMAKE_EXECUTABLE_SUFFIX}")

# Locate crashpad_handler from vcpkg tools
find_program(CRASHPAD_HANDLER_EXE NAMES crashpad_handler HINTS "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/tools")

if(APPLE)
  # Move crash handler executables inside Mac app bundle
  add_custom_command(TARGET arcvideo-crashhandler POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different arcvideo-crashhandler $<TARGET_FILE_DIR:arcvideo-editor>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CRASHPAD_HANDLER_EXE} $<TARGET_FILE_DIR:arcvideo-editor>
  )
elseif(WIN32)
  # Copy crashpad_handler next to arcvideo-editor.exe
  if(CRASHPAD_HANDLER_EXE)
    add_custom_command(TARGET arcvideo-crashhandler POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CRASHPAD_HANDLER_EXE}" $<TARGET_FILE_DIR:arcvideo-editor>
    )
  endif()
elseif(UNIX)
  install(TARGETS arcvideo-crashhandler RUNTIME DESTINATION bin)
  if(CRASHPAD_HANDLER_EXE)
    install(PROGRAMS ${CRASHPAD_HANDLER_EXE} DESTINATION bin)
  endif()
endif()

target_compile_definitions(arcvideo-crashhandler PRIVATE ${ARCVIDEO_DEFINITIONS})
