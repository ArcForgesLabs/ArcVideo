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

# --- OpenTimelineIO (optional, built from submodule) ---
if(ARCVIDEO_HAVE_OTIO)
  set(OTIO_SHARED_LIBS OFF CACHE INTERNAL "Build OTIO as static library")
  set(OTIO_PYTHON_INSTALL OFF CACHE INTERNAL "Disable OTIO Python bindings")
  set(OTIO_CXX_INSTALL OFF CACHE INTERNAL "Disable OTIO install targets")
  set(OTIO_DEPENDENCIES_INSTALL OFF CACHE INTERNAL "Disable OTIO deps install")
  set(OTIO_AUTOMATIC_SUBMODULES OFF CACHE INTERNAL "Don't auto-update submodules")
  set(OTIO_FIND_IMATH ON CACHE INTERNAL "Use vcpkg Imath instead of bundled")
  set(BUILD_TESTING OFF CACHE INTERNAL "Disable OTIO tests")
  add_subdirectory(OpenTimelineIO EXCLUDE_FROM_ALL)

  # OTIO uses PRIVATE includes for its src dir; expose for consumers
  target_include_directories(opentimelineio PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/OpenTimelineIO/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/OpenTimelineIO/src>
  )
  target_include_directories(opentime PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/OpenTimelineIO/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/OpenTimelineIO/src>
  )
endif()

set(KDDockWidgets_STATIC ON CACHE INTERNAL "Force KDDockWidgets to build statically")
set(KDDockWidgets_QT6 ON CACHE INTERNAL "Build KDDockWidgets with Qt6")
set(KDDockWidgets_FRONTENDS "qtwidgets" CACHE INTERNAL "Only build QtWidgets frontend")
set(KDDockWidgets_EXAMPLES OFF CACHE INTERNAL "Disable KDDockWidgets examples")
set(KDDockWidgets_TESTS OFF CACHE INTERNAL "Disable KDDockWidgets tests")

# KDDockWidgets needs Qt private modules
find_package(Qt6 REQUIRED COMPONENTS WidgetsPrivate)
if(WIN32)
  find_package(Qt6 REQUIRED COMPONENTS GuiPrivate)
endif()

add_subdirectory(KDDockWidgets EXCLUDE_FROM_ALL)
