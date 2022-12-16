// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ORDERSTABLEMODEL_H
#define ANTIQUACRM_ORDERSTABLEMODEL_H

#include <AntiquaCRM>
#include <QObject>
#include <QWidget>

class OrdersTableModel final : public AntiquaCRM::ASqlQueryModel {
  Q_OBJECT

private:
  QSqlRecord queryRecord;
  const QIcon getHeaderIcon(int column) const;
  const QIcon getPaymentIcon(int status = 0) const;
  const QString getPaymentStatus(int status) const;
  const QString getOrderStatus(int status) const;
  const QIcon getOrderStatusIcon(int status) const;
  const QString getRunTime(const qint64 &seconds) const;
  const QString getToolTip(int column) const;

public:
  explicit OrdersTableModel(QObject *parent = nullptr);
  const QMap<int, QString> headerList() const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
};

#endif // ANTIQUACRM_ORDERSTABLEMODEL_H
