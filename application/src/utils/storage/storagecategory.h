// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef STORAGE_CATEGORY_H
#define STORAGE_CATEGORY_H

#include <QComboBox>
#include <QCompleter>
#include <QFrame>
#include <QMap>
#include <QLineEdit>
#include <QObject>
#include <QVariant>
#include <QWidget>

/**
 * @brief The StorageCategory class
 * @ingroup _storage
 */
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
  void addItems(const QMap<QString,QString> &map);
  void setIndex(int index = 0);
  int getIndex();
  const QString getTitle();
  void setValue(qint64 cid);
  const QString getValue(qint64 cid = 0);
  int count();
};

#endif // STORAGE_CATEGORY_H
