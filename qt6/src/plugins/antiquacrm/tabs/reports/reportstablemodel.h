// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_REPORTS_TABLEMODEL_H
#define ANTIQUACRM_REPORTS_TABLEMODEL_H

#include "reportsconfig.h"
#include <AntiquaCRM>
#include <QObject>

class ANTIQUACRM_REPORTS_PLUGIN ReportsTableModel final : public AntiquaCRM::ASqlQueryModel {
  Q_OBJECT

public:
  explicit ReportsTableModel(QObject* parent = nullptr);
  const QMap<int, QString> headerIndex() const;
  const QMap<int, QString> headerList() const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  QVariant data(const QModelIndex& item, int role = Qt::DisplayRole) const override;
};

#endif // ANTIQUACRM_REPORTS_TABLEMODEL_H
