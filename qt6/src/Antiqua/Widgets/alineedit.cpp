// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "alineedit.h"

#include <QMetaType>

namespace AntiquaCRM {

ALineEdit::ALineEdit(QWidget *parent) : QLineEdit{parent}, AInputEdit{parent} {
  setClearButtonEnabled(true);
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

void ALineEdit::setRestrictions(const QSqlField &field) {
  QMetaType _type = field.metaType();
  int _length = field.length();
  if (_type.id() == QMetaType::QString && _length > 0) {
    setMaxLength(_length);
    QString info(tr("Max allowed length") + " ");
    info.append(QString::number(_length));
    if (placeholderText().isEmpty())
      setPlaceholderText(info);
  }

  if (field.requiredStatus() == QSqlField::Required) {
    if (objectName().contains("_0")) {
      setRequired(true);
      setClearButtonEnabled(false);
    }
  }
}

const QVariant ALineEdit::getValue() { return text().trimmed(); }

} // namespace AntiquaCRM
