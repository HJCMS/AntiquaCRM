// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "reportstoolbar.h"

#include <AntiquaWidgets>
#include <QCalendar>
#include <QSizePolicy>

ReportAction::ReportAction(const QIcon &icon, const QDate &date,
                           QObject *parent)
    : QAction{icon, date.toString("MMMM"), parent}, p_date{date} {

  connect(this, SIGNAL(triggered()), SLOT(clicked()));
}

void ReportAction::clicked() { emit signalClicked(p_date); }

ReportsToolBar::ReportsToolBar(QWidget *parent)
    : QToolBar{parent},
      p_icon{AntiquaCRM::antiquaIcon("view-calendar-timeline")} {

  m_info = new QLabel(this);
  m_info->setIndent(4);
  m_info->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  addWidget(m_info);

  m_months = new QPushButton(tr("Select report"), this);
  m_months->setIcon(p_icon);
  m_menu = new QMenu(m_months);
  m_months->setMenu(m_menu);
  addWidget(m_months);
  addSeparator();

  m_print = new QPushButton(tr("Print"), this);
  m_print->setIcon(AntiquaCRM::antiquaIcon("document-print"));
  connect(m_print, SIGNAL(clicked()), SIGNAL(sendPrintReport()));
  addWidget(m_print);
  addSeparator();

  m_save = new QPushButton(tr("Save"), this);
  m_save->setIcon(AntiquaCRM::antiquaIcon("document-save"));
  connect(m_save, SIGNAL(clicked()), SIGNAL(sendSaveReport()));
  addWidget(m_save);
}

void ReportsToolBar::selectionChanged(const QDate &date) {
  if (date.isValid()) {
    updateInfoLabel(date);
    emit signalSelected(date);
  }
}

void ReportsToolBar::updateInfoLabel(const QDate &date, double volume) {
  QString _i;
  if (volume > 0.00) {
    const QString _money = AntiquaCRM::ATaxCalculator::money(volume);
    _i = tr("Report for „%1“, total %2.")
             .arg(date.toString("MMMM yyyy"), _money);
  } else {
    _i = tr("Report for „%1“").arg(date.toString("MMMM yyyy"));
  }
  m_info->setText(_i);
}

const QDate ReportsToolBar::firstDayInMonth(const QDate &date) const {
  return QDate(date.year(), date.month(), 1);
}

const QDate ReportsToolBar::lastDayInMonth(const QDate &date) const {
  QCalendar cal(QCalendar::System::Gregorian);
  return QDate(date.year(), date.month(),
               cal.daysInMonth(date.month(), date.year()));
}

bool ReportsToolBar::initSelection(AntiquaCRM::ASqlCore *msql) {
  const QString _sql = AntiquaCRM::ASqlFiles::queryStatement(
      "statistics_from_until_delivery_year");
  QSqlQuery _q = msql->query(_sql);
  if (_q.size() > 0) {
    while (_q.next()) {
      const QDate _from = firstDayInMonth(_q.value("min").toDate());
      const QDate _until = lastDayInMonth(_q.value("max").toDate());
      for (int y = _from.year(); y <= _until.year(); y++) {
        QMenu *m_year = new QMenu(QString::number(y), m_months);
        m_year->setIcon(p_icon);
        for (int m = 1; m <= 12; m++) {
          const QDate _d = lastDayInMonth(QDate(y, m, 1));
          if (_d >= _from && _d <= _until) {
            // qDebug() << _d << _from << _until;
            ReportAction *ac = new ReportAction(p_icon, _d, m_year);
            connect(ac, SIGNAL(signalClicked(const QDate &)),
                    SLOT(selectionChanged(const QDate &)));
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
