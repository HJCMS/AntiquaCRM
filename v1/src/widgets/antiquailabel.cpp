// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquailabel.h"

AntiquaILabel::AntiquaILabel(QWidget *parent) : QLabel{parent} {
  setAlignment(Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);
  setVisible(false);
}
