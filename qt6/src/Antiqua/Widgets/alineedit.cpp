// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "alineedit.h"

#include <QMetaType>

namespace AntiquaCRM {

ALineEdit::ALineEdit(QWidget *parent) : QLineEdit{parent}, AInputEdit{parent} {
  connect(this, SIGNAL(returnPressed()), SLOT(skipReturnPressed()));
}

void ALineEdit::skipReturnPressed() {
  /* force to disable emit actions in dialogs */
}

void ALineEdit::setValue(const QVariant &value) {
  QString data;
  QMetaType _type = value.metaType();
  if (_type.id() == QMetaType::QString) {
    data = value.toString().trimmed();
  } else if (_type.id() == QMetaType::Int) {
    data = QString::number(value.toInt());
  }
  setText(data);
}

const QVariant ALineEdit::getValue() { return text().trimmed(); }

} // namespace AntiquaCRM
