// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "alineedit.h"

namespace AntiquaCRM {

ALineEdit::ALineEdit(QWidget *parent) : QLineEdit{parent} {
  setClearButtonEnabled(true);
  connect(this, SIGNAL(returnPressed()), SLOT(skipReturnPressed()));
}

void ALineEdit::skipReturnPressed() {
  qInfo("Press::Key Return skipped");
  setModified(true);
}

void ALineEdit::isValidContent(bool b) {
  if(b)
    setStyleSheet(QString());
  else
    setStyleSheet("QLineEdit {selection-background-color: red;}");
}

} // namespace AntiquaCRM
