// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef STORAGE_MODEL_H
#define STORAGE_MODEL_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>

/**
 * @brief The StorageModel class
 * @ingroup _storage
 */
class StorageModel : public QSqlTableModel {
  Q_OBJECT

public:
  explicit StorageModel(const QSqlDatabase &db, QObject *parent = nullptr);
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
};

#endif // STORAGE_MODEL_H
