// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_DESCWORDSTABLEMODEL_H
#define ANTIQUACRM_UTILS_DESCWORDSTABLEMODEL_H

#include <AntiquaCRM>
#include <QMap>
#include <QModelIndex>
#include <QObject>
#include <QVariant>

class DescWordsTableModel final : public AntiquaCRM::ASqlQueryModel {
  Q_OBJECT

public:
  explicit DescWordsTableModel(QObject* parent = nullptr);
  static const QString fieldList();
  static const QString tableName();
  static const QString orderColumn();
  static const QString sectionTitle(int);
  const QMap<int, QString> headerList() const override;
  QVariant headerData(int, Qt::Orientation, int role = Qt::DisplayRole) const override;
  QVariant data(const QModelIndex&, int role = Qt::DisplayRole) const override;
};

#endif // ANTIQUACRM_UTILS_DESCWORDSTABLEMODEL_H
