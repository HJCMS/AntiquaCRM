// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aproviderorders.h"

#include <QDate>
#include <QDateTime>
#include <QDebug>

namespace AntiquaCRM {

AProviderOrder::AProviderOrder(const QString &uniqId) {
  bookingId = uniqId;
  p_data = QHash<QString, QVariant>();
}

AProviderOrder::AProviderOrder(const AProviderOrder &other) {
  bookingId = other.bookingId;
  p_data = other.p_data;
}

AProviderOrder &AProviderOrder::operator=(const AProviderOrder &other) {
  if (this == &other)
    return *this;

  bookingId = other.bookingId;
  p_data = other.p_data;
  return *this;
}

const QStringList AProviderOrder::currentKeys() const {
  QStringList list;
  QHashIterator<QString, QVariant> it(p_data);
  while (it.hasNext()) {
    it.next();
    if (!it.value().isNull())
      list << it.key();
  }
  return list;
}

bool AProviderOrder::setValue(const QString &key, const QVariant &value) {
  QHashIterator<QString, QMetaType::Type> it(validKeys());
  while (it.hasNext()) {
    it.next();
    QMetaType type(it.value());
    if ((it.key() == key) && (type.name().contains(value.typeName()))) {
      if (type.id() == QMetaType::QString)
        p_data.insert(key, value.toString().trimmed());
      else
        p_data.insert(key, value);

      return true;
    }
  }
  return false;
}

const QVariant AProviderOrder::getValue(const QString &key) {
  if (count() < 1)
    return QVariant();

  QHashIterator<QString, QVariant> it(p_data);
  while (it.hasNext()) {
    it.next();
    if (it.key() == key) {
      return it.value();
    }
  }
  return QVariant();
}

}; // namespace AntiquaCRM
