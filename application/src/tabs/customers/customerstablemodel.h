// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_CUSTOMERSTABLEMODEL_H
#define ANTIQUACRM_CUSTOMERSTABLEMODEL_H

#include <AntiquaCRM>
#include <QObject>
#include <QIcon>
#include <QWidget>

/**
 * @brief Customers Table model
 * @ingroup _customers
 */
class CustomersTableModel final : public AntiquaCRM::ASqlQueryModel {
  Q_OBJECT

private:
  QSqlRecord queryRecord;
  const QIcon getHeaderIcon(int column) const;
  const QIcon getStatusIcon(int status = 0) const;
  const QString getToolTip(int column) const;

public:
  explicit CustomersTableModel(QObject *parent = nullptr);
  const QMap<int, QString> headerList() const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
};

#endif // ANTIQUACRM_CUSTOMERSTABLEMODEL_H
