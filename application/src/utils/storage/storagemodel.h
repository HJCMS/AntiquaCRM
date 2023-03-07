// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef STORAGE_MODEL_H
#define STORAGE_MODEL_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>

/**
 * @brief The StorageModel class
 * @ingroup AntiquaStorage
 */
class StorageModel : public QSqlTableModel {
  Q_OBJECT

public:
  explicit StorageModel(const QSqlDatabase &db, QObject *parent = nullptr);
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
};

#endif // STORAGE_MODEL_H
