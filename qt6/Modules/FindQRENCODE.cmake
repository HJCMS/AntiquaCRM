# Distributed under the OSI-approved BSD 3-Clause License.
# See accompanying file Copyright.txt or https://cmake.org/licensing for details.

#[=======================================================================[.rst:
FindQRENCODE
--------

Find the native libqrencode headers and libraries.

IMPORTED Targets
^^^^^^^^^^^^^^^^

This module defines :prop_tgt:`IMPORTED` target ``QRENCODE::libqrencode``, if
qrencode has been found.

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

if(QRENCODE_ROOT)
  set(_QRENCODE_SEARCH_ROOT PATHS ${QRENCODE_ROOT} NO_DEFAULT_PATH)
  list(APPEND _QRENCODE_PATHS _QRENCODE_SEARCH_ROOT)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)

find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
  pkg_check_modules(PKG_QRENCODE QUIET libqrencode)
  if(PKG_QRENCODE_FOUND)
    set(QRENCODE_FOUND BOOL ${PKG_QRENCODE_FOUND})
    set(QRENCODE_VERSION ${PKG_QRENCODE_VERSION})
    set(QRENCODE_INCLUDE_DIR ${PKG_QRENCODE_INCLUDE_DIRS})
    set(QRENCODE_LIBRARY ${PKG_QRENCODE_LINK_LIBRARIES})
  endif()
endif()

## BEGIN QRENCODE_LIBRARY
if(NOT QRENCODE_LIBRARY)
  ## Set base Library
  find_library(QRENCODE_LIBRARY
    NAMES qrencode libqrencode
    PATHS ${_QRENCODE_PATHS}
    PATH_SUFFIXES lib lib64
  )
  mark_as_advanced(QRENCODE_LIBRARY)

  find_library(QRENCODE_LIBRARY_REALEASE
    NAMES qrencode libqrencode
    PATHS ${_QRENCODE_PATHS}
    PATH_SUFFIXES lib lib64
  )
  mark_as_advanced(QRENCODE_LIBRARY_REALEASE)

  find_library(QRENCODE_LIBRARY_DEBUG
    NAMES qrencode-d qrencode-debug libqrencode-d libqrencode-debug
    PATHS ${_QRENCODE_PATHS}
    PATH_SUFFIXES lib lib64
  )
  mark_as_advanced(QRENCODE_LIBRARY_DEBUG)

  include(${CMAKE_CURRENT_LIST_DIR}/SelectLibraryConfigurations.cmake)

  select_library_configurations(qrencode)

endif(NOT QRENCODE_LIBRARY)
## END

## BEGIN QRENCODE_INCLUDE_DIR
if(NOT QRENCODE_INCLUDE_DIR)
  find_path(QRENCODE_INCLUDE_DIR NAMES qrencode.h PATH_SUFFIXES include)
endif(NOT QRENCODE_INCLUDE_DIR)
mark_as_advanced(QRENCODE_INCLUDE_DIR)
## END

FIND_PACKAGE_HANDLE_STANDARD_ARGS(QRENCODE
  REQUIRED_VARS QRENCODE_LIBRARY QRENCODE_INCLUDE_DIR
  VERSION_VAR QRENCODE_VERSION
  HANDLE_COMPONENTS
)

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
