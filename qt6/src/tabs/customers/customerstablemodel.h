// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_CUSTOMERSTABLEMODEL_H
#define ANTIQUACRM_PLUGIN_CUSTOMERSTABLEMODEL_H

#include <AntiquaCRM>
#include <QIcon>
#include <QModelIndex>
#include <QObject>

class CustomersTableModel final : public AntiquaCRM::ASqlQueryModel {
  Q_OBJECT

private:
  const QIcon headerIcon(int column) const;
  const QIcon trustIcon(int status) const;
  const QIcon lockIcon(bool locked = false) const;
  const QString toolTip(int column) const;

public:
  explicit CustomersTableModel(QObject* parent = nullptr);
  const QMap<int, QString> headerList() const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
};

#endif // ANTIQUACRM_PLUGIN_CUSTOMERSTABLEMODEL_H
