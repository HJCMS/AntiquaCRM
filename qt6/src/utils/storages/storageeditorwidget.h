// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_STORAGEEDITORWIDGET_H
#define ANTIQUACRM_UTILS_STORAGEEDITORWIDGET_H

#include <AntiquaWidgets>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QRegularExpression>
#include <QSqlRecord>
#include <QWidget>

#include "storageutil.h"

class StorageCategory;

class StorageEditorWidget final : public QWidget {
  Q_OBJECT

private:
  const QRegularExpression pattern;
  AntiquaCRM::NumEdit *sl_id;
  AntiquaCRM::TextLine *sl_storage;
  AntiquaCRM::TextLine *sl_identifier;
  AntiquaCRM::TextLine *sl_location;

  QGroupBox *m_providers;
  StorageCategory *m_abebooks;
  StorageCategory *m_booklooker;

  QLabel *subTitle(const QString &) const;
  bool check(AntiquaCRM::TextLine *);

  bool setAbeBooks();
  bool setBooklooker();
  void setCategory(const QString &name, qint64 cid);

private Q_SLOTS:
  void dataModified();

public Q_SLOTS:
  void setValue(const StorageItemData &items);
  void setRestrictions(const QSqlRecord &record);
  void clear();

public:
  explicit StorageEditorWidget(QWidget *parent = nullptr);
  void initDataset();
  const QString sqlQuery();
};

#endif // ANTIQUACRM_UTILS_STORAGEEDITORWIDGET_H
