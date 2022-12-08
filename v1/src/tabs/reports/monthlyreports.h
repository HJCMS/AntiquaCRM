// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_MONTHLY_REPORTS_H
#define ANTIQUACRM_MONTHLY_REPORTS_H

#include <QComboBox>
#include <QDate>
#include <QFileInfo>
#include <QObject>
#include <QWidget>

class MonthSelecter;
class MonthlyReportView;

class MonthlyReports final : public QWidget {
  Q_OBJECT

private:
  MonthSelecter *m_selecter;
  MonthlyReportView *m_table;
  QDate p_date;
  const QString printHeader();
  const QFileInfo getSaveFile();

private Q_SLOTS:
  void createReport(const QDate &);
  void printReport();
  void saveReport();

Q_SIGNALS:
  void sendSaveNotification(const QString &);

public Q_SLOTS:
  void updateVolumeInfo();

public:
  explicit MonthlyReports(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_MONTHLY_REPORTS_H
