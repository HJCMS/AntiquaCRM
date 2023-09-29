// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef PUBLISHERS_VIEW_MODEL_H
#define PUBLISHERS_VIEW_MODEL_H

#include <AntiquaCRM>
#include <QObject>
#include <QSqlTableModel>
#include <QWidget>

class PublishersViewModel final : public AntiquaCRM::ASqlQueryModel {
  Q_OBJECT

public:
  explicit PublishersViewModel(QObject *parent = nullptr);
  const QMap<int, QString> headerList() const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;

  /**
   * @brief EditData query
   * @param index - Row Index
   * @param role - HINT: Changed to default Qt::EditRole
   */
  QVariant data(const QModelIndex &index,
                int role = Qt::EditRole) const override;
};

#endif // PUBLISHERS_VIEW_MODEL_H
