// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_SELLERS_REPAIR_EDIT_H
#define ANTIQUACRM_SELLERS_REPAIR_EDIT_H

#include <AntiquaWidgets>
#include <QJsonObject>
#include <QLabel>
#include <QObject>
#include <QScrollArea>
#include <QSignalMapper>
#include <QToolButton>
#include <QWidget>

class ImportRepairEdit final : public QScrollArea {
  Q_OBJECT
  Q_PROPERTY(qint64 CustomerId READ getCustomerId WRITE setCustomerId
             NOTIFY signalCutomerIdChanged)

private:
  qint64 CustomerId = -1;
  QWidget *m_widget;
  AntiquaCRM::TextLine *o_provider_import;      // display unique key
  AntiquaCRM::TextLine *c_provider_import;      // display unique key
  AntiquaCRM::TextLine *o_firstname;            // origin firstname
  AntiquaCRM::TextLine *c_firstname;            // edit firstname
  AntiquaCRM::TextLine *o_lastname;             // origin lastname
  AntiquaCRM::TextLine *c_lastname;             // edit lastname
  AntiquaCRM::TextLine *c_email_0;              // edit email_0
  AntiquaCRM::PhoneEdit *c_phone_0;             // edit phone_0
  AntiquaCRM::TextLine *o_country;              // origin country
  AntiquaCRM::TextLine *c_country;              // edit country
  AntiquaCRM::SelectEUCountry *c_country_bcp47; // edit country_bcp47
  AntiquaCRM::TextLine *o_postalcode;           // origin postalcode
  AntiquaCRM::PostalCodeEdit *c_postalcode;     // edit postalcode
  AntiquaCRM::TextLine *o_location;             // origin location
  AntiquaCRM::TextLine *c_location;             // edit location
  AntiquaCRM::TextLine *o_street;               // origin street
  AntiquaCRM::TextLine *c_street;               // edit street
  AntiquaCRM::TextField *o_postal_address;      // origin postal_address
  AntiquaCRM::TextField *c_postal_address;      // edit postal_address
  AntiquaCRM::TextField *o_shipping_address;    // origin shipping_address
  AntiquaCRM::TextField *c_shipping_address;    // edit shipping_address
  QSignalMapper *m_mapper;
  QLabel *info(const QString &title);
  QToolButton *mergeButton(const QString &suffix);

private Q_SLOTS:
  void mergeData(const QString &id);

Q_SIGNALS:
  void signalCutomerIdChanged();
  void sendFindClause(const QString &clause);

public Q_SLOTS:
  void setCustomerId(qint64);

public:
  explicit ImportRepairEdit(QWidget *parent = nullptr);
  qint64 getCustomerId();
  void setOriginData(const QJsonObject &data);
  void setImportData(const QJsonObject &data);
  const QJsonObject getData();
};

#endif // ANTIQUACRM_SELLERS_REPAIR_EDIT_H
