// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "reportstoolbar.h"
#include "reportaction.h"
#include <AntiquaWidgets>
#include <QCalendar>
#include <QLocale>
#include <QSizePolicy>

inline const QIcon _micon = AntiquaCRM::antiquaIcon("view-calendar-timeline");

ReportsToolBar::ReportsToolBar(QWidget* parent) : QToolBar{parent}, p_lc{QLocale::system()} {
  m_info = new QLabel(this);
  m_info->setIndent(4);
  m_info->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  addWidget(m_info);

  btn_months = new QPushButton(tr("Select report"), this);
  btn_months->setIcon(_micon);
  m_menu = new QMenu(btn_months);
  btn_months->setMenu(m_menu);
  addWidget(btn_months);
  addSeparator();

  btn_print = new QPushButton(tr("Print"), this);
  btn_print->setIcon(AntiquaCRM::antiquaIcon("document-print"));
  connect(btn_print, SIGNAL(clicked()), SIGNAL(sendPrintReport()));
  addWidget(btn_print);
  addSeparator();

  btn_save = new QPushButton(tr("Save"), this);
  btn_save->setIcon(AntiquaCRM::antiquaIcon("document-save"));
  connect(btn_save, SIGNAL(clicked()), SIGNAL(sendSaveReport()));
  addWidget(btn_save);
}

void ReportsToolBar::selectionChanged(const QDate& date) {
  if (date.isValid()) {
    updateInfoLabel(date);
    emit signalSelected(date);
  }
}

void ReportsToolBar::updateInfoLabel(const QDate& date, double volume) {
  QString _i;
  const QString _month = p_lc.monthName(date.month(), QLocale::LongFormat);
  const QString _year = date.toString("yyyy");
  if (volume > 0) {
    const QString _money = AntiquaCRM::ATaxCalculator::money(volume);
    _i = tr("Report for „%1 %2“, total %3.").arg(_month, _year, _money);
  } else {
    _i = tr("Report for „%1 %2“").arg(_month, _year);
  }
  m_info->setText(_i);
}

const QDate ReportsToolBar::firstDayInMonth(const QDate& date) const {
  return QDate(date.year(), date.month(), 1);
}

const QDate ReportsToolBar::lastDayInMonth(const QDate& date) const {
  QCalendar cal(QCalendar::System::Gregorian);
  return QDate(date.year(), date.month(), cal.daysInMonth(date.month(), date.year()));
}

bool ReportsToolBar::initSelection(AntiquaCRM::ASqlCore* msql) {
  const QString _sql = AntiquaCRM::ASqlFiles::queryStatement("statistics_from_until_delivery_year");
  QSqlQuery _q = msql->query(_sql);
  if (_q.size() > 0) {
    while (_q.next()) {
      const QDate _from = firstDayInMonth(_q.value("min").toDate());
      const QDate _until = lastDayInMonth(_q.value("max").toDate());
      for (int y = _from.year(); y <= _until.year(); y++) {
        QMenu* m_year = new QMenu(QString::number(y), btn_months);
        m_year->setIcon(_micon);
        for (int m = 1; m <= 12; m++) {
          const QDate _d = lastDayInMonth(QDate(y, m, 1));
          if (_d >= _from && _d <= _until) {
            // qDebug() << _d << _from << _until;
            const QString _title = p_lc.monthName(_d.month(), QLocale::LongFormat);
            ReportAction* ac = new ReportAction(_d, _title, m_year);
            connect(ac, SIGNAL(signalClicked(QDate)), SLOT(selectionChanged(QDate)));
            m_year->addAction(ac);
          }
        }
        m_menu->addMenu(m_year);
      }
      _q.clear();
    }
    return true;
  }
  return false;
}
