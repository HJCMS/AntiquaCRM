# Distributed under the OSI-approved BSD 3-Clause License.
# See accompanying file Copyright.txt or https://cmake.org/licensing for details.

#[=======================================================================[.rst:
FindCDIO
--------

Find the native libcdio headers and libraries.

IMPORTED Targets
^^^^^^^^^^^^^^^^

This module defines :prop_tgt:`IMPORTED` target ``CDIO::libcdio``, if
cdio has been found.

Result Variables
^^^^^^^^^^^^^^^^

This module defines the following variables:

``CDIO_FOUND``
  "True" if ``libcdio`` found.

``CDIO_INCLUDE_DIRS``
  where to find ``include``/cdio/``cdio.h``, etc.

``CDIO_LIBRARIES``
  List of libraries when using ``libcdio``.

#]=======================================================================]

include(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)

find_path(CDIO_INCLUDE_DIR
  NAMES cdio/cdio.h
  HINTS ${PC_CDIO_INCLUDE_DIRS})
mark_as_advanced(CDIO_INCLUDE_DIR)

if(NOT CDIO_LIBRARY)
  find_library(CDIO_LIBRARY NAMES
    cdio
    libcdio
    iso9660
    libiso9660
    HINTS ${PC_CDIO_LIBRARY_DIRS}
  )
  find_library(CDIO_LIBRARY_RELEASE NAMES
    cdio
    libcdio
    iso9660
    libiso9660
    HINTS ${PC_CDIO_LIBRARY_DIRS}
  )
  mark_as_advanced(CDIO_LIBRARY_RELEASE)
  find_library(CDIO_LIBRARY_DEBUG NAMES
    cdio-d
    libcdio-d
    cdio_debug
    libcdio_debug
    iso9660-d
    libiso9660-d
    iso9660_debug
    libiso9660_debug
    HINTS ${PC_CDIO_LIBRARY_DIRS}
  )
  mark_as_advanced(CDIO_LIBRARY_DEBUG)
  include(${CMAKE_CURRENT_LIST_DIR}/SelectLibraryConfigurations.cmake)
  select_library_configurations(CDIO)
endif(NOT CDIO_LIBRARY)

find_package_handle_standard_args(CDIO
  REQUIRED_VARS CDIO_LIBRARY CDIO_INCLUDE_DIR
  VERSION_VAR CDIO_VERSION)

if(CDIO_FOUND)
  set(CDIO_LIBRARIES ${CDIO_LIBRARY})
  set(CDIO_INCLUDE_DIRS ${CDIO_INCLUDE_DIR})
  set(CDIO_VERSION ${CDIO_VERSION})

  if(NOT TARGET CDIO::libcdio)
    add_library(CDIO::libcdio UNKNOWN IMPORTED)
    set_target_properties(CDIO::libcdio PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${CDIO_INCLUDE_DIRS}")

    if(EXISTS "${CDIO_LIBRARY}")
      set_target_properties(CDIO::libcdio PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        IMPORTED_LOCATION "${CDIO_LIBRARY}")
    endif()
    if(CDIO_LIBRARY_RELEASE)
      set_property(TARGET CDIO::libcdio APPEND PROPERTY
        IMPORTED_CONFIGURATIONS RELEASE)
      set_target_properties(CDIO::libcdio PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        IMPORTED_LOCATION_RELEASE "${CDIO_LIBRARY_RELEASE}")
    endif()
    if(CDIO_LIBRARY_DEBUG)
      set_property(TARGET CDIO::libcdio APPEND PROPERTY
        IMPORTED_CONFIGURATIONS DEBUG)
      set_target_properties(CDIO::libcdio PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        IMPORTED_LOCATION_DEBUG "${CDIO_LIBRARY_DEBUG}")
    endif()
  endif()
endif()
