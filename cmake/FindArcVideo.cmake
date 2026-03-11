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

set(LIBARCVIDEO_COMPONENTS
  Core
  #Codec
)

foreach (COMPONENT ${LIBARCVIDEO_COMPONENTS})
  string(TOLOWER ${COMPONENT} LOWER_COMPONENT)
  string(TOUPPER ${COMPONENT} UPPER_COMPONENT)

  # Find include directory for this component
  find_path(LIBARCVIDEO_${UPPER_COMPONENT}_INCLUDEDIR
      arcvideo/${LOWER_COMPONENT}/${LOWER_COMPONENT}.h
    HINTS
      "${LIBARCVIDEO_LOCATION}"
      "$ENV{LIBARCVIDEO_LOCATION}"
      "${LIBARCVIDEO_ROOT}"
      "$ENV{LIBARCVIDEO_ROOT}"
    PATH_SUFFIXES
      include/
  )

  find_library(LIBARCVIDEO_${UPPER_COMPONENT}_LIBRARY
      arcvideo${LOWER_COMPONENT}
    HINTS
      "${LIBARCVIDEO_LOCATION}"
      "$ENV{LIBARCVIDEO_LOCATION}"
      "${LIBARCVIDEO_ROOT}"
      "$ENV{LIBARCVIDEO_ROOT}"
    PATH_SUFFIXES
      lib/
  )

  list(APPEND LIBARCVIDEO_LIBRARIES ${LIBARCVIDEO_${UPPER_COMPONENT}_LIBRARY})
  list(APPEND LIBARCVIDEO_INCLUDE_DIRS ${LIBARCVIDEO_${UPPER_COMPONENT}_INCLUDEDIR})
endforeach()

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(ArcVideo
  REQUIRED_VARS
    LIBARCVIDEO_LIBRARIES
    LIBARCVIDEO_INCLUDE_DIRS
)
