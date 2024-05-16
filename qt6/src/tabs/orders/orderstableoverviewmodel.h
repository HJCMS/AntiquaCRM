// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_ORDERSTABLEOVERVIEWMODEL_H
#define ANTIQUACRM_PLUGIN_ORDERSTABLEOVERVIEWMODEL_H

#include <AntiquaWidgets>
#include <QSqlRecord>
#include <QObject>

class ANTIQUACRM_LIBRARY OrdersTableOverViewModel final
    : public AntiquaCRM::ASqlQueryModel {
  Q_OBJECT

private:
  const QIcon getHeaderIcon(int column) const;
  const QIcon getPaymentIcon(int status = 0) const;
  const QString getPaymentStatus(int status) const;
  const QString getOrderStatus(int status) const;
  const QIcon getOrderStatusIcon(int status) const;
  const QString getRunTime(const qint64 &seconds) const;
  const QString getToolTip(int column) const;

public:
  explicit OrdersTableOverViewModel(QObject *parent = nullptr);
  const QMap<int, QString> headerList() const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
};

#endif // ANTIQUACRM_PLUGIN_ORDERSTABLEOVERVIEWMODEL_H
