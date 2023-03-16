// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "adateinfo.h"

#include <QDate>
#include <QDateTime>
#include <QMetaType>

#ifndef FALLBACK_DATE
#define FALLBACK_DATE QDate(2000, 01, 01)
#endif

namespace AntiquaCRM {

ADateInfo::ADateInfo(QWidget *parent)
    : QDateTimeEdit{parent}, AInputEdit{parent} {
  setButtonSymbols(QAbstractSpinBox::NoButtons);
  setDisplayFormat(ANTIQUACRM_DATETIME_DISPLAY);
  setInputMethodHints(Qt::ImhNone);
  setReadOnly(true);
  setStyleSheet("QDateTimeEdit {border:none;background:transparent;}");
}

void ADateInfo::setValue(const QVariant &value) {
  QMetaType _type = value.metaType();
  if (value.isNull()) {
    setDate(FALLBACK_DATE);
  } else if (_type.id() == QMetaType::QDate) {
    setDate(value.toDate());
  } else if (_type.id() == QMetaType::QDateTime) {
    setDateTime(value.toDateTime());
  } else if (_type.id() == QMetaType::QString) {
    QString str = value.toString().trimmed();
    QDateTime dt = QDateTime::fromString(str, ANTIQUACRM_TIMESTAMP_IMPORT);
    if (dt.isValid()) {
      setDateTime(dt);
    }
  }
}

const QVariant ADateInfo::getValue() { return dateTime(); }

} // namespace AntiquaCRM
