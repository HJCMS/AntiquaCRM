// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "reportsindex.h"
#include "printreport.h"
#include "reportstableview.h"
#include "reportstoolbar.h"

#include <AntiquaPrinting>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QLocale>
#include <QVBoxLayout>

ReportsIndex::ReportsIndex(QWidget *parent)
    : AntiquaCRM::TabsIndex{"reports_tab", parent} {
  setObjectName("reports_tab_widget");
  setWindowIcon(AntiquaCRM::antiquaIcon("x-office-spreadsheet"));
  setWindowTitle(getTitle());
  setClosable(true);

  // Begin::MainView
  QWidget *m_mainWidget = new QWidget(this);
  QVBoxLayout *mainLayout = new QVBoxLayout(m_mainWidget);
  m_table = new ReportsTableView(this);
  mainLayout->insertWidget(0, m_table);
  m_toolBar = new ReportsToolBar(m_mainWidget);
  mainLayout->insertWidget(1, m_toolBar);
  mainLayout->setStretch(0, 1);
  m_mainWidget->setLayout(mainLayout);
  insertWidget(0, m_mainWidget);
  // End::MainView

  setCurrentIndex(ViewPage::MainView);

  connect(m_toolBar, SIGNAL(signalSelected(const QDate &)),
          SLOT(createReport(const QDate &)));
  connect(m_toolBar, SIGNAL(sendPrintReport()), SLOT(printReport()));
  connect(m_toolBar, SIGNAL(sendSaveReport()), SLOT(saveReport()));
}

void ReportsIndex::setDefaultTableView() {}

const QString ReportsIndex::getBasename() {
  QString _name(p_date.toString("yyyy-MM"));
  _name.append("_" + tr("report") + "_");
  QDateTime _ts = QDateTime::currentDateTime();
  _name.append(_ts.toString("ddMMyyhh"));
  _name.append("_utf8");
  return _name;
}

const QString ReportsIndex::printHeader() {
  AntiquaCRM::ASettings cfg(this);
  QString info = cfg.value("company_fullname", "").toString();
  info.append("\n");
  info.append(tr("Report for "));
  info.append(p_date.toString("MMM MM.yyyy") + ".");
  return info;
}

const QFileInfo ReportsIndex::getSaveFile() {
  AntiquaCRM::ASettings cfg(this);
  QDir p_dir(cfg.getArchivPath(ANTIQUACRM_ARCHIVE_REPORTS));
  return QFileInfo(p_dir, getBasename());
}

void ReportsIndex::createReport(const QDate &date) {
  AntiquaCRM::ASqlFiles _tpl("query_monthly_report");
  if (!_tpl.openTemplate())
    return;

  QString _sql("o_delivered BETWEEN '");
  _sql.append(date.toString("01.MM.yyyy") + "T00:00:00");
  _sql.append("' AND '");
  _sql.append(date.toString("dd.MM.yyyy") + "T23:59:59");
  _sql.append("'");
  _tpl.setWhereClause(_sql);

  p_date = date;
  m_table->setQuery(_tpl.getQueryContent());
  m_toolBar->updateInfoLabel(date, m_table->salesVolume());
}

void ReportsIndex::printReport() {
  QJsonObject obj = m_table->printingData();
  obj.insert("info", printHeader());
  obj.insert("file", getBasename());

  AntiquaCRM::PrintReport *m_p = new AntiquaCRM::PrintReport(this);
  if (m_p->exec(obj, true) == QDialog::Accepted) {
    qInfo("Report printed");
  }
  m_p->deleteLater();
}

void ReportsIndex::saveReport() {
  QFileInfo dest = getSaveFile();
  if (!dest.dir().exists() || dest.fileName().isEmpty()) {
    qWarning("Save target not exists or no file name!");
    return;
  }

  QString header = m_table->dataHeader();
  QStringList rows = m_table->csvExport();
  if (rows.count() > 0) {
    QString filePath = dest.filePath().append(".csv");
    QFile fp(filePath);
    if (fp.open(QIODevice::WriteOnly)) {
      QTextStream out(&fp);
      out.setLocale(QLocale::system());
      out << header + QChar::LineFeed;
      out << rows.join(QChar::LineFeed) + QChar::LineFeed;
      fp.close();
#ifdef ANTIQUA_DEVELOPEMENT
      qDebug() << "Save:" << dest.filePath();
#endif
      emit sendStatusMessage(tr("Report saved: %1").arg(dest.fileName()));
    }
  }
}

void ReportsIndex::onEnterChanged() {
  if (initialed)
    return;

  if (m_sql == nullptr)
    m_sql = new AntiquaCRM::ASqlCore(this);

  initialed = m_toolBar->initSelection(m_sql);
  if (initialed) // current month
    createReport(m_toolBar->lastDayInMonth(QDate::currentDate()));
}

const QString ReportsIndex::getTitle() const { return tr("Reports"); }

bool ReportsIndex::customAction(const QJsonObject &) { return false; }

const QStringList ReportsIndex::acceptsCustomActions() const {
  return QStringList();
}
