// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "creditworthiness.h"

Creditworthiness::Creditworthiness(QWidget *parent) : QLabel{parent} {
  QString _txt = tr("<b>Warning</b>: Check Customer creditworthiness!");
  setAlignment(Qt::AlignLeft | Qt::AlignVCenter);;
  setIndent(5),
  setText(_txt);
  setVisible(false);
}
