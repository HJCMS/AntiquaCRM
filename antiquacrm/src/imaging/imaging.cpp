// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imaging.h"
#include "version.h"

#include <QtCore/QDebug>

Imaging::Imaging(QWidget *parent) : QLabel{parent} {
  setObjectName("Imaging");
  setBackgroundRole(QPalette::Base);
  setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  setScaledContents(true);
}
