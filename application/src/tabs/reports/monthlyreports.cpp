// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "monthlyreports.h"
#include "monthlyreportview.h"
#include "monthselecter.h"

#include <ASettings>
#include <AntiquaPrinting>
#include <QCalendar>
#include <QDir>
#include <QFile>
#include <QLabel>
#include <QLocale>
#include <QVBoxLayout>

// TODO EDI: https://www.gs1-germany.de/gs1-standards/datenaustausch/eancom/
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
  filename.append(p_date.toString("yyyy-MM"));
  filename.append("_" + tr("report") + "_");
  QDateTime timeStamp = QDateTime::currentDateTime();
  filename.append(timeStamp.toString("ddMMyyhh"));
  filename.append("_utf8");
  QFileInfo info(QDir(p_dir), filename);
  return info;
}

void MonthlyReports::createReport(const QDate &date) {
  QCalendar cal(QCalendar::System::Gregorian);
  p_date = QDate(date.year(), date.month(),
                 cal.daysInMonth(date.month(), date.year()));

  if (!p_date.isValid()) {
    qWarning("Selected Report Date is invalid!");
    return;
  }

  QString where_clause("o_delivered BETWEEN '");
  QDate fromDay(p_date.year(), p_date.month(), 1);
  where_clause.append(fromDay.toString("dd.MM.yyyy") + "T00:00:00");
  where_clause.append("' AND '");
  where_clause.append(p_date.toString("dd.MM.yyyy") + "T23:59:00");
  where_clause.append("'");

  AntiquaCRM::ASqlFiles file("query_monthly_report");
  if (file.openTemplate()) {
    file.setWhereClause(where_clause);
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << file.getQueryContent();
#endif
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
      out.setLocale(QLocale::system());
      out.setCodec(QTextCodec::codecForLocale()->name());
      out << header + QChar::LineFeed;
      out << rows.join(QChar::LineFeed) + QChar::LineFeed;
      fp.close();
#ifdef ANTIQUA_DEVELOPEMENT
      qDebug() << "Export:" << dest.filePath();
#endif
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
