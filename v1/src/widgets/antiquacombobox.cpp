// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquacombobox.h"

/*
#include <QDebug>
#include <QApplication>
#include <QAbstractItemView>
*/

AntiquaComboBox::AntiquaComboBox(QWidget *parent) : QComboBox{parent} {
  setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
//  QColor col = qApp->palette().color(QPalette::AlternateBase);
//  setStyleSheet("* {alternate-background-color: " + col.name() + ";}");
//  view()->setAlternatingRowColors(true);
}

void AntiquaComboBox::wheelEvent(QWheelEvent *e) {
  if (mouseWheel) {
    QComboBox::wheelEvent(e);
  }
}

void AntiquaComboBox::setMouseWheelEnabled(bool b) {
  mouseWheel = b;
  emit sendMouseWheelChanged();
}

bool AntiquaComboBox::isMouseWheelEnabled() { return mouseWheel; }
