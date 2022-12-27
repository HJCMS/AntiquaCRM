// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "monthlyreports.h"
#include "monthlyreportview.h"
#include "monthselecter.h"

#include <ASettings>
#include <AntiquaPrinting>
#include <QDir>
#include <QFile>
#include <QLabel>
#include <QLayout>

MonthlyReports::MonthlyReports(QWidget *parent) : QWidget{parent} {
  setContentsMargins(0, 0, 0, 0);
  QVBoxLayout *layout = new QVBoxLayout(this);

  p_date = QDate::currentDate();

  QString title = tr("Monthly Payment reports");
  layout->addWidget(new QLabel(title, this));

  m_table = new MonthlyReportView(this);
  layout->addWidget(m_table);

  m_selecter = new MonthSelecter(this);
  layout->addWidget(m_selecter);

  layout->setStretch(1, 1);
  setLayout(layout);

  connect(m_table, SIGNAL(sendFinished()), SLOT(updateVolumeInfo()));

  connect(m_selecter, SIGNAL(sendMonthDate(const QDate &)),
          SLOT(createReport(const QDate &)));

  connect(m_selecter, SIGNAL(sendPrintReport()), SLOT(printReport()));
  connect(m_selecter, SIGNAL(sendSaveReport()), SLOT(saveReport()));
}

const QString MonthlyReports::printHeader() {
  AntiquaCRM::ASettings cfg(this);
  QString total = m_table->salesVolume();
  QString info = cfg.value("company_fullname", "").toString();
  info.append("\n");
  info.append(tr("Report for "));
  info.append(p_date.toString("MMM MM.yyyy") + " - ");
  info.append(tr("total sales") + " " + total);
  info.append(cfg.value("payment/currency", "$").toString());
  return info;
}

const QFileInfo MonthlyReports::getSaveFile() {
  AntiquaCRM::ASettings cfg(this);
  QDir p_dir(cfg.value("dirs/reports", QDir::homePath()).toString());
  QString filename;
  filename.append(p_date.toString("MMMM_yyyy"));
  filename.append("_" + tr("report") + "_");
  QDateTime timeStamp = QDateTime::currentDateTime();
  filename.append(timeStamp.toString("ddMMyyhhmm"));
  filename.append("_utf8");
  QFileInfo info(QDir(p_dir), filename);
  return info;
}

void MonthlyReports::createReport(const QDate &date) {
  p_date = date;
  QString where_clause("o_delivered BETWEEN '");
  QDate fromDate(date.year(), date.month(), 1);
  where_clause.append(fromDate.toString("dd.MM.yyyyT00:00:00"));
  where_clause.append("' AND '");
  where_clause.append(date.toString("dd.MM.yyyyT23:59:00"));
  where_clause.append("'");

  AntiquaCRM::ASqlFiles file("query_monthly_report");
  if (file.openTemplate()) {
    file.setWhereClause(where_clause);
    m_table->setQuery(file.getQueryContent());
  }
}

void MonthlyReports::printReport() {
  QStringList list = m_table->dataRows();
  QString info = printHeader();
  if (list.size() > 0 && !info.isEmpty()) {
    Reports *m_print = new Reports(this);
    m_print->setHeaderInfo(info, m_table->dataHeader());
    m_print->exec(list);
  }
}

void MonthlyReports::saveReport() {
  QFileInfo dest = getSaveFile();
  if (!dest.dir().exists() || dest.fileName().isEmpty()) {
    qWarning("Save target not exists or no file name!");
    return;
  }

  QString header = m_table->dataHeader();
  QStringList rows = m_table->dataRows();
  if (rows.count() > 0) {
    QString filePath = dest.filePath().append(".csv");
    QFile fp(filePath);
    if (fp.open(QIODevice::WriteOnly)) {
      QTextStream out(&fp);
      out.setCodec("UTF8");
      out << header + "\n";
      out << rows.join("\n");
      fp.close();
      emit sendSaveNotification(tr("Report saved: %1").arg(dest.fileName()));
    }
  }
}

void MonthlyReports::updateVolumeInfo() {
  AntiquaCRM::ASettings cfg(this);
  QString info = tr("Volume");
  info.append(" " + m_selecter->selectedText() + " = ");
  info.append(m_table->salesVolume() + " ");
  info.append(cfg.value("payment/currency", "$").toString());
  m_selecter->showMessage(info);
}
