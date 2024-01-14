# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: MIT
#

## API-Documentation
SET(doxygen_FILES
 Core/Core.dox
 Providers/Providers.dox
 Widgets/Widgets.dox
 Doxyfile.cmake
)

CONFIGURE_FILE (Doxyfile.cmake
 ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
)

# 1) Core Library
ADD_SUBDIRECTORY (Core)

# 2) Widgets Library
SET (INTERNAL_WIDGET_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}/Core
)
ADD_SUBDIRECTORY (Widgets)

# 3) Library Include dirs
SET (INTERNAL_LIBRARY_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}/Core
  ${CMAKE_CURRENT_SOURCE_DIR}/Widgets
)
ADD_SUBDIRECTORY (Printing)
ADD_SUBDIRECTORY (Image)
ADD_SUBDIRECTORY (Providers)
ADD_SUBDIRECTORY (Tabs)

#EOF
