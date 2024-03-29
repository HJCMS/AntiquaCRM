// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DELIVERITEM_H
#define DELIVERITEM_H

#include <QtGlobal>
#include <QDateTime>
#include <QString>

/**
 * @ingroup _deliveryservice
 * @brief Deliver Item class
 */
class DeliverItem final {
private:
  const int d_srv;
  const QString d_class;
  int d_cid;
  QString d_definition;
  QString d_description;
  bool d_international;
  bool d_default;
  qreal d_price;
  QDateTime d_changed;

public:
  explicit DeliverItem(int index, const QString &name);
  int index() const;
  const QString name() const;

  void setPrimaryKey(int id);
  int primaryKey();

  void setDefinition(const QString &);
  const QString definition();

  void setInternational(bool t = false);
  bool international();

  void setDefault(bool b = false);
  bool isDefault();

  void setDescription(const QString &);
  const QString description();

  void setPrice(qreal);
  qreal price();

  void setChanged(const QDateTime &);
  const QDateTime lastChanged();
};

#endif // DELIVERITEM_H
