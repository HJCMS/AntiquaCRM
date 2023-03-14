# Distributed under the OSI-approved BSD 3-Clause License.
# See accompanying file Copyright.txt or https://cmake.org/licensing for details.

#[=======================================================================[.rst:
FindDISCID
--------

Find the native libdiscid headers and libraries.

IMPORTED Targets
^^^^^^^^^^^^^^^^

This module defines :prop_tgt:`IMPORTED` target ``DISCID::libdiscid``, if
discid has been found.

Result Variables
^^^^^^^^^^^^^^^^

This module defines the following variables:

``DISCID_FOUND``
  "True" if ``libdiscid`` found.

``DISCID_INCLUDE_DIRS``
  where to find ``include``/discid/``discid.h``, etc.

``DISCID_LIBRARIES``
  List of libraries when using ``libdiscid``.

#]=======================================================================]

include(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)

find_path(DISCID_INCLUDE_DIR
  NAMES discid/discid.h
  HINTS ${PC_DISCID_INCLUDE_DIRS})
mark_as_advanced(DISCID_INCLUDE_DIR)

if(NOT DISCID_LIBRARY)
  find_library(DISCID_LIBRARY NAMES
    discid
    libdiscid
    HINTS ${PC_DISCID_LIBRARY_DIRS}
  )
  find_library(DISCID_LIBRARY_RELEASE NAMES
    discid
    libdiscid
    HINTS ${PC_DISCID_LIBRARY_DIRS}
  )
  mark_as_advanced(DISCID_LIBRARY_RELEASE)
  find_library(DISCID_LIBRARY_DEBUG NAMES
    discid-d
    libdiscid-d
    discid_debug
    libdiscid_debug
    HINTS ${PC_DISCID_LIBRARY_DIRS}
  )
  mark_as_advanced(DISCID_LIBRARY_DEBUG)
  include(${CMAKE_CURRENT_LIST_DIR}/SelectLibraryConfigurations.cmake)
  select_library_configurations(DISCID)
endif(NOT DISCID_LIBRARY)

find_package_handle_standard_args(DISCID
  REQUIRED_VARS DISCID_LIBRARY DISCID_INCLUDE_DIR
  VERSION_VAR DISCID_VERSION)

if(DISCID_FOUND)
  set(DISCID_LIBRARIES ${DISCID_LIBRARY})
  set(DISCID_INCLUDE_DIRS ${DISCID_INCLUDE_DIR})
  set(DISCID_VERSION ${DISCID_VERSION})

  if(NOT TARGET DISCID::libdiscid)
    add_library(DISCID::libdiscid UNKNOWN IMPORTED)
    set_target_properties(DISCID::libdiscid PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${DISCID_INCLUDE_DIRS}")

    if(EXISTS "${DISCID_LIBRARY}")
      set_target_properties(DISCID::libdiscid PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        IMPORTED_LOCATION "${DISCID_LIBRARY}")
    endif()
    if(DISCID_LIBRARY_RELEASE)
      set_property(TARGET DISCID::libdiscid APPEND PROPERTY
        IMPORTED_CONFIGURATIONS RELEASE)
      set_target_properties(DISCID::libdiscid PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        IMPORTED_LOCATION_RELEASE "${DISCID_LIBRARY_RELEASE}")
    endif()
    if(DISCID_LIBRARY_DEBUG)
      set_property(TARGET DISCID::libdiscid APPEND PROPERTY
        IMPORTED_CONFIGURATIONS DEBUG)
      set_target_properties(DISCID::libdiscid PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        IMPORTED_LOCATION_DEBUG "${DISCID_LIBRARY_DEBUG}")
    endif()
  endif()
endif()
