// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_CDVTABLEMODEL_H
#define ANTIQUACRM_CDVTABLEMODEL_H

#include <AntiquaCRM>
#include <QObject>
#include <QWidget>

/**
 * @brief CD+Vinyl Table model
 * @ingroup _cdvinyl
 */
class CDVTableModel final : public AntiquaCRM::ASqlQueryModel {
  Q_OBJECT

private:
  QSqlRecord queryRecord;

public:
  explicit CDVTableModel(QObject *parent = nullptr);
  const QMap<int, QString> headerList() const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  QVariant data(const QModelIndex &item,
                int role = Qt::DisplayRole) const override;
};

#endif // ANTIQUACRM_CDVTABLEMODEL_H
