// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "reportstoolbar.h"

#include <AntiquaWidgets>
#include <QCalendar>

ReportAction::ReportAction(const QIcon &icon, const QDate &date,
                           QObject *parent)
    : QAction{icon, date.toString("MMMM"), parent} {

  QCalendar cal(QCalendar::System::Gregorian);
  p_date = QDate(date.year(), date.month(),
                 cal.daysInMonth(date.month(), date.year()));

  connect(this, SIGNAL(triggered()), SLOT(clicked()));
}

void ReportAction::clicked() { emit signalClicked(p_date); }

ReportsToolBar::ReportsToolBar(QWidget *parent)
    : QToolBar{parent},
      p_icon{AntiquaCRM::antiquaIcon("view-calendar-timeline")} {
  setLayoutDirection(Qt::RightToLeft);

  m_save = new QPushButton(tr("Save"), this);
  m_save->setLayoutDirection(Qt::LeftToRight);
  m_save->setIcon(AntiquaCRM::antiquaIcon("document-save"));
  connect(m_save, SIGNAL(clicked()), SIGNAL(sendSaveReport()));
  addWidget(m_save);
  addSeparator();

  m_print = new QPushButton(tr("Print"), this);
  m_print->setLayoutDirection(Qt::LeftToRight);
  m_print->setIcon(AntiquaCRM::antiquaIcon("document-print"));
  connect(m_print, SIGNAL(clicked()), SIGNAL(sendPrintReport()));
  addWidget(m_print);
  addSeparator();

  m_months = new QPushButton(tr("Select report"), this);
  m_months->setLayoutDirection(Qt::LeftToRight);
  m_months->setIcon(p_icon);
  m_menu = new QMenu(m_months);
  m_months->setMenu(m_menu);
  addWidget(m_months);
}

bool ReportsToolBar::initSelection(AntiquaCRM::ASqlCore *msql) {
  const QString _sql = AntiquaCRM::ASqlFiles::queryStatement(
      "statistics_from_until_delivery_year");
  QSqlQuery _q = msql->query(_sql);
  if (_q.size() > 0) {
    while (_q.next()) {
      const QDate _from(_q.value("min").toDate());
      const QDate _until(_q.value("max").toDate());
      for (int y = _from.year(); y <= _until.year(); y++) {
        QMenu *m_year = new QMenu(QString::number(y), m_months);
        m_year->setIcon(p_icon);
        for (int m = 1; m <= 12; m++) {
          const QDate _d(y, m, 1);
          if (_d >= _from && _d <= _until) {
            ReportAction *ac = new ReportAction(p_icon, _d, m_year);
            connect(ac, SIGNAL(signalClicked(const QDate &)),
                    SIGNAL(signalSelected(const QDate &)));
            m_year->addAction(ac);
          }
        }
        m_menu->addMenu(m_year);
      }
    }
    return true;
  }
  return false;
}
