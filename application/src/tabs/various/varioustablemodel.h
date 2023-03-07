// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_VARIOUSTABLEMODEL_H
#define ANTIQUACRM_VARIOUSTABLEMODEL_H

#include <AntiquaCRM>
#include <QObject>
#include <QWidget>

/**
 * @brief Inventory Various Table model
 * @ingroup InventoryVarious
 */
class VariousTableModel final : public AntiquaCRM::ASqlQueryModel {
  Q_OBJECT

private:
  QSqlRecord queryRecord;

public:
  explicit VariousTableModel(QObject *parent = nullptr);
  const QMap<int, QString> headerList() const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  QVariant data(const QModelIndex &item,
                int role = Qt::DisplayRole) const override;
};

#endif // ANTIQUACRM_VARIOUSTABLEMODEL_H
