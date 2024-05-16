// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_PUBLISHERTABLEMODEL_H
#define ANTIQUACRM_UTILS_PUBLISHERTABLEMODEL_H

#include <AntiquaCRM>
#include <QObject>
#include <QWidget>

class PublisherTableModel final : public AntiquaCRM::ASqlQueryModel {
  Q_OBJECT

public:
  explicit PublisherTableModel(QObject *parent = nullptr);
  const QMap<int, QString> headerList() const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;

  QVariant data(const QModelIndex &index,
                int role = Qt::EditRole) const override;
};

#endif // ANTIQUACRM_UTILS_PUBLISHERTABLEMODEL_H
