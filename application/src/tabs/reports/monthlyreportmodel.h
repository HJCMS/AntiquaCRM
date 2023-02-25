// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_MONTHLY_REPORTMODEL_H
#define ANTIQUACRM_MONTHLY_REPORTMODEL_H

#include <AntiquaCRM>
#include <QObject>

class MonthlyReportModel final : public AntiquaCRM::ASqlQueryModel {
  Q_OBJECT

public:
  explicit MonthlyReportModel(QObject *parent = nullptr);
  const QMap<int, QString> headerList() const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  QVariant data(const QModelIndex &item,
                int role = Qt::DisplayRole) const override;
};

#endif // ANTIQUACRM_MONTHLY_REPORTMODEL_H
