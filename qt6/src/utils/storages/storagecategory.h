// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_STORAGECATEGORY_H
#define ANTIQUACRM_UTILS_STORAGECATEGORY_H

#include <QComboBox>
#include <QCompleter>
#include <QFrame>
#include <QLineEdit>
#include <QMap>
#include <QObject>
#include <QVariant>
#include <QWidget>

class StorageCategory final : public QFrame {
  Q_OBJECT

private:
  QComboBox *m_box;
  QLineEdit *m_find;
  QCompleter *m_completer;

private Q_SLOTS:
  void onKeyEnterPressed();

public Q_SLOTS:
  void searchCategory(const QString &);
  void restore();
  void clear();

public:
  explicit StorageCategory(QWidget *parent = nullptr);
  void addItems(const QMap<QString, QString> &map);
  void setIndex(int index = 0);
  int getIndex();
  const QString getTitle();
  void setValue(qint64 cid);
  const QString getValue(qint64 cid = 0);
  int count();
};

#endif // ANTIQUACRM_UTILS_STORAGECATEGORY_H
