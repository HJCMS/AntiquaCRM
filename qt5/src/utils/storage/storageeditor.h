// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef STORAGE_EDITOR_H
#define STORAGE_EDITOR_H

#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QRegularExpression>
#include <QSpinBox>
#include <QWidget>

#include "storagetable.h"

class StorageCategory;

/**
 * @brief The StorageEditor class
 * @ingroup _storage
 */
class StorageEditor final : public QWidget {
  Q_OBJECT

private:
  const QRegularExpression pattern;
  QSpinBox *sl_id;
  QLineEdit *sl_storage;
  QLineEdit *sl_identifier;
  QLineEdit *sl_location;

  QGroupBox *m_providers;
  StorageCategory *m_abebooks;
  StorageCategory *m_booklooker;

  QLabel *subTitle(const QString &) const;
  bool check(QLineEdit *);

  bool setAbeBooks();
  bool setBooklooker();

  const QString getCategoryId(const QString &name);
  void setCategory(const QString &name, qint64 cid);

public Q_SLOTS:
  void setValue(const StorageTable::RowValues &items);
  void clear();

public:
  StorageEditor(QWidget *parent = nullptr);
  void initDataset();
  const QString sqlQuery();
};

#endif // STORAGE_EDITOR_H
