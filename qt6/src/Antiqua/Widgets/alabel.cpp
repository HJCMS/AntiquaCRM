// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "alabel.h"

#include <QMetaType>
#ifdef ANTIQUA_DEVELOPEMENT
#include <QDebug>
#endif

namespace AntiquaCRM {

ALabel::ALabel(QWidget *parent) : QLabel{parent}, AInputEdit{parent} {}

ALabel::ALabel(const QString &text, QWidget *parent) : ALabel{parent} {
  setText(text);
}

void ALabel::setValue(const QVariant &value) {
  QMetaType _type = value.metaType();
  if (_type.id() == QMetaType::QString) {
    QString str = value.toString().trimmed();
    if (str.isEmpty())
      return;

    setText(str);
  } else if (_type.id() == QMetaType::Int) {
    QString str = QString::number(value.toInt());
    if (str.isEmpty())
      return;

    setText(str);
  }
#ifdef ANTIQUA_DEVELOPEMENT
  else {
    qDebug() << Q_FUNC_INFO << "Invalid MimeType" << value;
  }
#endif
}

const QVariant ALabel::getValue() { return text(); }

} // namespace AntiquaCRM
