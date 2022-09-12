// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#include "antiquawindow.h"

AntiquaWindow::AntiquaWindow(QWidget *parent) : QMainWindow{parent} {
  setMinimumSize(QSize(500, 450));
  setWindowTitle("AntiquaCRM [*]");
}
