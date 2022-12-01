// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabreports.h"
#include "monthlyreports.h"

#include <QDebug>

TabReports::TabReports(QWidget *parent) : Inventory{"reports_tab", parent} {
  setWindowTitle(tr("Reports"));
  setWindowIcon(getTabIcon("view_log"));
  setObjectName("antiqua_reports_tab");
  setClosable(true);

  m_pageStart = new QWidget(this);
  insertWidget(0, m_pageStart);

  m_monthlyReports = new MonthlyReports(this);
  insertWidget(1, m_monthlyReports);

  connect(m_monthlyReports, SIGNAL(sendSaveNotification(const QString &)),
          SLOT(sendStatusMessage(const QString &)));
}

void TabReports::openStartPage() { setCurrentIndex(0); }

void TabReports::createSearchQuery(const QString &query) {
  // MONTHLY_REPORT
  if (query == "MONTHLY_REPORT") {
    setCurrentIndex(1);
  } else {
    qDebug() << Q_FUNC_INFO << query;
    setCurrentIndex(0);
  }
}

void TabReports::onEnterChanged() {}
