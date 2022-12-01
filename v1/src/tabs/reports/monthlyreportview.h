// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_MONTHLY_REPORTVIEW_H
#define ANTIQUACRM_MONTHLY_REPORTVIEW_H

#include <AntiquaCRM>
#include <QHeaderView>
#include <QObject>
#include <QTableView>
#include <QWidget>

class MonthlyReportModel final : public AntiquaCRM::ASqlQueryModel {
  Q_OBJECT

public:
  explicit MonthlyReportModel(QObject *parent = nullptr);
  const QMap<int, QString> headerList() const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
};

class MonthlyReportView : public QTableView {
  Q_OBJECT

private:
  int calc_section = -1;
  MonthlyReportModel *m_model;
  QHeaderView *m_tableHeader;

Q_SIGNALS:
  void sendFinished();

public Q_SLOTS:
  void setQuery(const QString &query);

public:
  explicit MonthlyReportView(QWidget *parent = nullptr);
  const QString headerName(const QString &key);
  const QString dataHeader(const QString &delimiter = QString(";"));
  const QStringList dataRows(const QString &delimiter = QString(";"));
  const QString salesVolume();
};

#endif // ANTIQUACRM_MONTHLY_REPORTVIEW_H
