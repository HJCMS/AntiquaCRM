// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_CDSVINYLTABLEMODEL_H
#define ANTIQUACRM_PLUGIN_CDSVINYLTABLEMODEL_H

#include <AntiquaCRM>
#include <QModelIndex>
#include <QObject>
#include <QSqlRecord>

class ANTIQUACRM_LIBRARY CDsVinylTableModel final
    : public AntiquaCRM::ASqlQueryModel {
  Q_OBJECT

private:
  QSqlRecord queryRecord;

public:
  explicit CDsVinylTableModel(QObject *parent = nullptr);
  const QMap<int, QString> headerList() const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  QVariant data(const QModelIndex &item,
                int role = Qt::DisplayRole) const override;
};

#endif // ANTIQUACRM_PLUGIN_CDSVINYLTABLEMODEL_H
