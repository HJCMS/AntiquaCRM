// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aproviderorders.h"

#include <QCryptographicHash>
#include <QDate>
#include <QDateTime>
#include <QDebug>

namespace AntiquaCRM {

AProviderOrder::AProviderOrder(const QString &provider, const QString &uniqId) {
  providerName = provider;
  bookingId = uniqId;
  p_data = QHash<QString, QVariant>();
  p_orderItems = QList<OrderArticleItems>();
}

AProviderOrder::AProviderOrder(const AProviderOrder &other) {
  providerName = other.providerName;
  bookingId = other.bookingId;
  p_data = other.p_data;
  p_orderItems = other.p_orderItems;
}

AProviderOrder &AProviderOrder::operator=(const AProviderOrder &other) {
  if (this == &other)
    return *this;

  providerName = other.providerName;
  bookingId = other.bookingId;
  p_data = other.p_data;
  p_orderItems = other.p_orderItems;
  return *this;
}

const QString AProviderOrder::md5sum() const {
  QByteArray buf = QString(provider().toLower() + "_" + id()).toLocal8Bit();
  return QCryptographicHash::hash(buf, QCryptographicHash::Md5).toHex();
}

const QRegExp AProviderOrder::keysPattern() {
  return QRegExp("^c_[a-z0-9_]+$");
}

const QHash<QString, QMetaType::Type> AProviderOrder::customerKeys() {
  QHash<QString, QMetaType::Type> list;
  QHashIterator<QString, QMetaType::Type> it(orderEditKeys());
  while (it.hasNext()) {
    it.next();
    if (it.key().contains(keysPattern()))
      list.insert(it.key(), it.value());
  }
  return list;
}

const QHash<QString, QMetaType::Type> AProviderOrder::orderKeys() {
  QHash<QString, QMetaType::Type> list;
  QHashIterator<QString, QMetaType::Type> it(orderEditKeys());
  while (it.hasNext()) {
    it.next();
    if (!it.key().contains(keysPattern()))
      list.insert(it.key(), it.value());
  }
  return list;
}

const QStringList AProviderOrder::filledKeys() const {
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
  QHashIterator<QString, QMetaType::Type> it(orderEditKeys());
  while (it.hasNext()) {
    it.next();
    QMetaType type(it.value());
    if ((it.key() == key) && (type.name().contains(value.typeName()))) {
      if (type.id() == QMetaType::QString)
        p_data.insert(key, value.toString().trimmed());
      else if (type.id() == QMetaType::Int)
        p_data.insert(key, value.toInt());
      else if (type.id() == QMetaType::Bool)
        p_data.insert(key, value.toBool());
      else if (type.id() == QMetaType::QDateTime)
        p_data.insert(key, value.toDateTime());
      else if (type.id() == QMetaType::Double)
        p_data.insert(key, value.toInt());
      else
        p_data.insert(key, value);

      return true;
    } else if ((it.key() == key) && (value.type() == QVariant::LongLong)) {
      p_data.insert(key, value.toInt());
      return true;
    } else if ((it.key() == key) && (value.type() == QVariant::Double)) {
      p_data.insert(key, value.toDouble());
      return true;
    } else if ((it.key() == key) && (value.type() == QVariant::DateTime)) {
      p_data.insert(key, value.toDateTime());
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

AntiquaCRM::ArticleOrderItem AProviderOrder::createItem(const QString &key,
                                                        const QVariant &value) {
  AntiquaCRM::ArticleOrderItem item;
  item.key = key;
  item.value = value;
  return item;
}

const QList<OrderArticleItems> AProviderOrder::orders() { return p_orderItems; }

bool AProviderOrder::insertOrderItems(const OrderArticleItems &article) {
  p_orderItems.append(article);
  return true;
}

bool AProviderOrder::removeOrderItem(const QString &orderItemId) {
  if (p_orderItems.size() < 1) {
    qWarning("AProviderOrderItems ist empty, nothing to remove!");
    return false;
  }

  QListIterator<OrderArticleItems> main(p_orderItems);
  int i = 0;
  while (main.hasNext()) {
    QListIterator<ArticleOrderItem> sub(main.next());
    while (sub.hasNext()) {
      ArticleOrderItem item = sub.next();
      if (item.key == orderItemId) {
#ifdef ANTIQUA_DEVELOPEMENT
        qDebug() << Q_FUNC_INFO << i << item.key;
#endif
        p_orderItems.removeAt(i);
        return true;
      }
    }
    i++;
  }
  return false;
}

}; // namespace AntiquaCRM
