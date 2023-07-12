// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configgridlayout.h"
#include "awhatsthisbutton.h"

namespace AntiquaCRM {

ConfigGridLayout::ConfigGridLayout(QWidget *parent) : QGridLayout{parent} {
  setContentsMargins(5, 5, 5, 5);
}

void ConfigGridLayout::addToolTip(int row, int column, const QString &text) {
  AntiquaCRM::AWhatsThisButton *btn =
      new AntiquaCRM::AWhatsThisButton(text, parentWidget());
  btn->setFocusPolicy(Qt::NoFocus);
  addWidget(btn, row, column, 1, 1, Qt::AlignRight);
}

} // namespace AntiquaCRM
