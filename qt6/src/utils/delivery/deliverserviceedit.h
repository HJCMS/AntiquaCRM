// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_DELIVERSERVICEEDIT_H
#define ANTIQUACRM_UTILS_DELIVERSERVICEEDIT_H

#include <AntiquaWidgets>
#include <QComboBox>
#include <QLabel>
#include <QMap>
#include <QWidget>

class DeliverItem;
class DeliverServiceBox;

class DeliverServiceEdit : public QWidget {
  Q_OBJECT

private:
  AntiquaCRM::NumEdit *d_cid;
  DeliverServiceBox *d_srv;
  AntiquaCRM::TextLine *d_class;
  AntiquaCRM::TextLine *d_definition;
  AntiquaCRM::TextLine *d_description;
  AntiquaCRM::ALabel *d_infopage;
  AntiquaCRM::ALabel *d_website;
  AntiquaCRM::BoolBox *d_international;
  AntiquaCRM::BoolBox *d_default;
  AntiquaCRM::PriceEdit *d_price;
  AntiquaCRM::DateTimeInfo *d_changed;

  QLabel *info(const QString &);

private Q_SLOTS:
  void clearFields();

Q_SIGNALS:
  void message(const QString &str);

public Q_SLOTS:
  void createSubEntry(int id);
  void createNewEntry();
  void setServiceBoxFocus();

public:
  explicit DeliverServiceEdit(QWidget *parent = nullptr);
  const DeliverItem getSaveData();
  void addDeliverServiceEntry(const QMap<QString, QVariant> &entry);
};

#endif // ANTIQUACRM_UTILS_DELIVERSERVICEEDIT_H
