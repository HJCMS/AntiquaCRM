// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_VARIOUSTABLEMODEL_H
#define ANTIQUACRM_PLUGIN_VARIOUSTABLEMODEL_H

#include <AntiquaCRM>
#include <QMap>
#include <QModelIndex>
#include <QObject>
#include <QVariant>

class ANTIQUACRM_LIBRARY VariousTableModel final
    : public AntiquaCRM::ASqlQueryModel {
  Q_OBJECT

public:
  explicit VariousTableModel(QObject *parent = nullptr);
  const QMap<int, QString> headerList() const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  QVariant data(const QModelIndex &item,
                int role = Qt::DisplayRole) const override;
};

#endif // ANTIQUACRM_PLUGIN_VARIOUSTABLEMODEL_H
