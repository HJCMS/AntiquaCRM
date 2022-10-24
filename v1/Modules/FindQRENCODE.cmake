# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#[=======================================================================[.rst:
FindQREncode
--------

Find the native libqrencode headers and libraries.

IMPORTED Targets
^^^^^^^^^^^^^^^^

This module defines :prop_tgt:`IMPORTED` target ``QRENCODE::libqrencode``, if
curl has been found.

Result Variables
^^^^^^^^^^^^^^^^

This module defines the following variables:

``QRENCODE_FOUND``
  "True" if ``libqrencode`` found.

``QRENCODE_INCLUDE_DIRS``
  where to find ``include``/``qrencode.h``, etc.

``QRENCODE_LIBRARIES``
  List of libraries when using ``libqrencode``.

#]=======================================================================]

include(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)

find_path(QRENCODE_INCLUDE_DIR
  NAMES qrencode.h
  HINTS ${PC_QRENCODE_INCLUDE_DIRS})
mark_as_advanced(QRENCODE_INCLUDE_DIR)

if(NOT QRENCODE_LIBRARY)
  find_library(QRENCODE_LIBRARY NAMES
    qrencode
    libqrencode
    HINTS ${PC_QRENCODE_LIBRARY_DIRS}
  )
  find_library(QRENCODE_LIBRARY_RELEASE NAMES
    qrencode
    libqrencode
    HINTS ${PC_QRENCODE_LIBRARY_DIRS}
  )
  mark_as_advanced(QRENCODE_LIBRARY_RELEASE)
  find_library(QRENCODE_LIBRARY_DEBUG NAMES
    qrencode-d
    libqrencode-d
    qrencode_debug
    libqrencode_debug
    HINTS ${PC_QRENCODE_LIBRARY_DIRS}
  )
  mark_as_advanced(QRENCODE_LIBRARY_DEBUG)
  include(${CMAKE_CURRENT_LIST_DIR}/SelectLibraryConfigurations.cmake)
  select_library_configurations(QRENCODE)
endif(NOT QRENCODE_LIBRARY)

find_package_handle_standard_args(QRENCODE
  REQUIRED_VARS QRENCODE_LIBRARY QRENCODE_INCLUDE_DIR
  VERSION_VAR QRENCODE_VERSION)

if(QRENCODE_FOUND)
  set(QRENCODE_LIBRARIES ${QRENCODE_LIBRARY})
  set(QRENCODE_INCLUDE_DIRS ${QRENCODE_INCLUDE_DIR})
  set(QRENCODE_VERSION ${QRENCODE_VERSION})

  if(NOT TARGET QRENCODE::libqrencode)
    add_library(QRENCODE::libqrencode UNKNOWN IMPORTED)
    set_target_properties(QRENCODE::libqrencode PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${QRENCODE_INCLUDE_DIRS}")

    if(EXISTS "${QRENCODE_LIBRARY}")
      set_target_properties(QRENCODE::libqrencode PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        IMPORTED_LOCATION "${QRENCODE_LIBRARY}")
    endif()
    if(QRENCODE_LIBRARY_RELEASE)
      set_property(TARGET QRENCODE::libqrencode APPEND PROPERTY
        IMPORTED_CONFIGURATIONS RELEASE)
      set_target_properties(QRENCODE::libqrencode PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        IMPORTED_LOCATION_RELEASE "${QRENCODE_LIBRARY_RELEASE}")
    endif()
    if(QRENCODE_LIBRARY_DEBUG)
      set_property(TARGET QRENCODE::libqrencode APPEND PROPERTY
        IMPORTED_CONFIGURATIONS DEBUG)
      set_target_properties(QRENCODE::libqrencode PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        IMPORTED_LOCATION_DEBUG "${QRENCODE_LIBRARY_DEBUG}")
    endif()
  endif()
endif()
