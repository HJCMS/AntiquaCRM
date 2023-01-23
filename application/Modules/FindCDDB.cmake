# Distributed under the OSI-approved BSD 3-Clause License.
# See accompanying file Copyright.txt or https://cmake.org/licensing for details.

#[=======================================================================[.rst:
FindCDDB
--------

Find the native libcddb headers and libraries.

IMPORTED Targets
^^^^^^^^^^^^^^^^

This module defines :prop_tgt:`IMPORTED` target ``CDDB::libcddb``, if
cdio has been found.

Result Variables
^^^^^^^^^^^^^^^^

This module defines the following variables:

``CDDB_FOUND``
  "True" if ``libcddb`` found.

``CDDB_INCLUDE_DIRS``
  where to find ``include``/cddb/``cddb.h``, etc.

``CDDB_LIBRARIES``
  List of libraries when using ``libcddb``.

#]=======================================================================]

include(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)

find_path(CDDB_INCLUDE_DIR
  NAMES cddb/cddb.h
  HINTS ${PC_CDDB_INCLUDE_DIRS})
mark_as_advanced(CDDB_INCLUDE_DIR)

if(NOT CDDB_LIBRARY)
  find_library(CDDB_LIBRARY NAMES
    cdio
    libcddb
    HINTS ${PC_CDDB_LIBRARY_DIRS}
  )
  find_library(CDDB_LIBRARY_RELEASE NAMES
    cdio
    libcddb
    HINTS ${PC_CDDB_LIBRARY_DIRS}
  )
  mark_as_advanced(CDDB_LIBRARY_RELEASE)
  find_library(CDDB_LIBRARY_DEBUG NAMES
    cdio-d
    libcddb-d
    cdio_debug
    libcddb_debug
    HINTS ${PC_CDDB_LIBRARY_DIRS}
  )
  mark_as_advanced(CDDB_LIBRARY_DEBUG)
  include(${CMAKE_CURRENT_LIST_DIR}/SelectLibraryConfigurations.cmake)
  select_library_configurations(CDDB)
endif(NOT CDDB_LIBRARY)

find_package_handle_standard_args(CDDB
  REQUIRED_VARS CDDB_LIBRARY CDDB_INCLUDE_DIR
  VERSION_VAR CDDB_VERSION)

if(CDDB_FOUND)
  set(CDDB_LIBRARIES ${CDDB_LIBRARY})
  set(CDDB_INCLUDE_DIRS ${CDDB_INCLUDE_DIR})
  set(CDDB_VERSION ${CDDB_VERSION})

  if(NOT TARGET CDDB::libcddb)
    add_library(CDDB::libcddb UNKNOWN IMPORTED)
    set_target_properties(CDDB::libcddb PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${CDDB_INCLUDE_DIRS}")

    if(EXISTS "${CDDB_LIBRARY}")
      set_target_properties(CDDB::libcddb PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        IMPORTED_LOCATION "${CDDB_LIBRARY}")
    endif()
    if(CDDB_LIBRARY_RELEASE)
      set_property(TARGET CDDB::libcddb APPEND PROPERTY
        IMPORTED_CONFIGURATIONS RELEASE)
      set_target_properties(CDDB::libcddb PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        IMPORTED_LOCATION_RELEASE "${CDDB_LIBRARY_RELEASE}")
    endif()
    if(CDDB_LIBRARY_DEBUG)
      set_property(TARGET CDDB::libcddb APPEND PROPERTY
        IMPORTED_CONFIGURATIONS DEBUG)
      set_target_properties(CDDB::libcddb PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        IMPORTED_LOCATION_DEBUG "${CDDB_LIBRARY_DEBUG}")
    endif()
  endif()
endif()
