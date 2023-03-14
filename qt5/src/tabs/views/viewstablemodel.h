// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_VIEWSTABLEMODEL_H
#define ANTIQUACRM_VIEWSTABLEMODEL_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QWidget>

/**
 * @brief Inventory Views table model
 * @ingroup tabs
 */
class ViewsTableModel final : public QSqlTableModel {
  Q_OBJECT

protected:
  const QVariant verticalDisplayHeader(int section,
                                       int role = Qt::DisplayRole) const;

public:
  explicit ViewsTableModel(const QSqlDatabase &db, QObject *parent = nullptr);
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  const QString getLabelText();
};

#endif // ANTIQUACRM_VIEWSTABLEMODEL_H
