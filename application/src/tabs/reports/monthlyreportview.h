// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_MONTHLY_REPORTVIEW_H
#define ANTIQUACRM_MONTHLY_REPORTVIEW_H

#include <AntiquaCRM>
#include <QChar>
#include <QHeaderView>
#include <QObject>
#include <QTableView>
#include <QWidget>

class MonthlyReportModel;

/**
 * @brief The MonthlyReportView class
 * @ingroup _reports
 */
class MonthlyReportView : public QTableView {
  Q_OBJECT

private:
  int calc_section = -1;
  int refunds_section = -1;
  MonthlyReportModel *m_model;
  QHeaderView *m_tableHeader;

Q_SIGNALS:
  void sendFinished();

public Q_SLOTS:
  void setQuery(const QString &query);

public:
  explicit MonthlyReportView(QWidget *parent = nullptr);
  const QString headerName(const QString &key);
  const QString dataHeader(const QChar &delimiter = QChar::Tabulation);
  const QStringList dataRows(const QChar &delimiter = QChar::Tabulation);
  const QString salesVolume();
};

#endif // ANTIQUACRM_MONTHLY_REPORTVIEW_H
