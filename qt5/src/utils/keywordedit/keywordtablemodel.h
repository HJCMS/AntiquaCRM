// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef KEYWORD_TABLEMODEL_H
#define KEYWORD_TABLEMODEL_H

#include <QModelIndex>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QVariant>
#include <QWidget>

/**
 * @ingroup _keywordedit
 * @brief The KeywordTableModel class
 */
class KeywordTableModel : public QSqlTableModel {
  Q_OBJECT

public:
  explicit KeywordTableModel(const QSqlDatabase &db, QObject *parent = nullptr);
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
};

#endif // KEYWORD_TABLEMODEL_H
