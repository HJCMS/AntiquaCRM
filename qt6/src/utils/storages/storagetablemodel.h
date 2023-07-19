// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_STORAGETABLEMODEL_H
#define ANTIQUACRM_UTILS_STORAGETABLEMODEL_H

#include <AntiquaCRM>

class StorageTableModel final : public AntiquaCRM::ASqlQueryModel {
  Q_OBJECT

protected:
  const QMap<int, QString> headerList() const override;

public:
  explicit StorageTableModel(const QString &table, QObject *parent = nullptr);

  const QString statement(const QString &custom = QString()) const;

  bool select();

  bool update(const QString &sql);

  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  QVariant data(const QModelIndex &item,
                int role = Qt::DisplayRole) const override;
};

#endif // ANTIQUACRM_UTILS_STORAGETABLEMODEL_H
