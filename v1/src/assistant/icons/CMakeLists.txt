# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET (icon_FILES
  ../../icons/action_add.png
  ../../icons/action_cancel.png
  ../../icons/action_clear.png
  ../../icons/action_ok.png
  ../../icons/action_quit.png
  ../../icons/action_redo.png
  ../../icons/action_reload.png
  ../../icons/action_remove.png
  ../../icons/action_restore.png
  ../../icons/action_save.png
  ../../icons/action_search.png
  ../../icons/action_undo.png
  ../../icons/antiqua.png
  ../../icons/configure.png
)

FILE (COPY ${icon_FILES} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})
