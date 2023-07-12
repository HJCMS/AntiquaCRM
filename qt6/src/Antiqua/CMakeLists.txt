# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: LGPL-3.0-or-later
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

# 2) Providers Libraries
SET (INTERNAL_PROVIDERS_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}/Core
  ${CMAKE_CURRENT_SOURCE_DIR}/Widgets
)
ADD_SUBDIRECTORY (Providers)

# 3) Tabs Interface
SET (INTERNAL_TABS_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}/Core
  ${CMAKE_CURRENT_SOURCE_DIR}/Widgets
)
ADD_SUBDIRECTORY (Tabs)

#EOF
