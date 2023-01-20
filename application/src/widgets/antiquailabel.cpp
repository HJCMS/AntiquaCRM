// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquailabel.h"

AntiquaILabel::AntiquaILabel(QWidget *parent) : QLabel{parent} {
  setAlignment(Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);
  setVisible(false);
}

AntiquaILabel::AntiquaILabel(const QString &text, QWidget *parent)
    : AntiquaILabel{parent} {
  setValue(text);
}

void AntiquaILabel::setValue(const QVariant &value) {
  if (value.type() == QVariant::String) {
    QString str = value.toString().trimmed();
    if (str.isEmpty())
      return;

    setVisible(true);
    setText(str);
  }
}
