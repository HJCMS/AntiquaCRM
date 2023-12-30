// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_VIEWS_TABLEMODEL_H
#define ANTIQUACRM_VIEWS_TABLEMODEL_H

#include <AntiquaCRM>
#include <QModelIndex>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>

class ANTIQUACRM_LIBRARY ViewsTableModel final : public QSqlTableModel {
  Q_OBJECT

protected:
  const QVariant verticalDisplayHeader(int section,
                                       int role = Qt::DisplayRole) const;

public:
  explicit ViewsTableModel(const QSqlDatabase &db, QObject *parent = nullptr);
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
};

#endif // ANTIQUACRM_VIEWS_TABLEMODEL_H
