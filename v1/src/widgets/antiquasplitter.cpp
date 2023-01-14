// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquasplitter.h"

AntiquaSplitter::AntiquaSplitter(QWidget *parent)
    : QSplitter{Qt::Horizontal, parent} {
  setChildrenCollapsible(false);
  setContentsMargins(0, 0, 0, 0);
  QStringList css("::handle {margin:4px;padding:0px;}");
  css.append("::handle:pressed {background-color:palette(highlight);}");
  setStyleSheet(css.join(" "));
}

void AntiquaSplitter::addLeft(QWidget *widget) { insertWidget(0, widget); }

void AntiquaSplitter::addRight(QWidget *widget) { insertWidget(1, widget); }
