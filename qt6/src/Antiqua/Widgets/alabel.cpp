// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "alabel.h"

#include <QDate>
#include <QDateTime>
#include <QMetaType>
#ifdef ANTIQUA_DEVELOPEMENT
#include <QDebug>
#endif

namespace AntiquaCRM {

ALabel::ALabel(QWidget *parent) : QLabel{parent}, AInputEdit{parent} {
  setIndent(2);
  setTextFormat(Qt::PlainText);
  setTextInteractionFlags(Qt::TextSelectableByKeyboard |
                          Qt::TextSelectableByMouse);
}

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
    return;
  }

  switch (_type.id()) {
  case (QMetaType::Int):
  case (QMetaType::Long):
  case (QMetaType::LongLong): {
    qint64 _value = value.toInt();
    setText(QString::number(_value));
  } break;

  case (QMetaType::Float):
  case (QMetaType::Double): {
    double _value = value.toDouble();
    setText(QString::number(_value, 'f', 2));
  } break;

  case (QMetaType::QDate): {
    QDate _value = value.toDate();
    setText(_value.toString(ANTIQUACRM_DATETIME_DISPLAY));
  } break;

  case (QMetaType::QDateTime): {
    QDateTime _value = value.toDateTime();
    _value.setTimeSpec(Qt::LocalTime);
    setText(_value.toString(ANTIQUACRM_DATETIME_DISPLAY));
  } break;

  default:
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "Unknown MimeType" << value;
#endif
    break;
  }
}

void ALabel::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() != QSqlField::Required)
    return; // not required

  if (field.defaultValue().isNull())
    return; // nothing todo

  if (!text().trimmed().isEmpty())
    return; // nothing todo

  switch (field.metaType().id()) {
  case (QMetaType::QString):
  case (QMetaType::Int):
  case (QMetaType::Long):
  case (QMetaType::LongLong):
  case (QMetaType::Float):
  case (QMetaType::Double): {
    setValue(field.defaultValue());
    setRequired(true);
  } break;

  default:
    break;
  }
}

const QVariant ALabel::getValue() { return text(); }

} // namespace AntiquaCRM
