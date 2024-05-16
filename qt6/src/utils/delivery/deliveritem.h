// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_DELIVERITEM_H
#define ANTIQUACRM_UTILS_DELIVERITEM_H

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
  double d_price;
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

  void setPrice(double);
  double price();

  void setChanged(const QDateTime &);
  const QDateTime lastChanged();
};

#endif // ANTIQUACRM_UTILS_DELIVERITEM_H
