// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "daterangewidget.h"
#include "antiqua_global.h"

#include <QDate>
#include <QDateTime>
#include <QLayout>

DateRangeWidget::DateRangeWidget(QWidget *parent) : QWidget{parent} {
  QGridLayout *layout = new QGridLayout(this);

  layout->addWidget(setLabel(tr("From Date")), 0, 0);
  layout->addWidget(setLabel(tr("To Date")), 0, 1);

  QDate cd = QDate::currentDate();
  QDate d(cd.year(), (cd.month() - 1), 1);

  m_fromDate = new QCalendarWidget(this);
  m_fromDate->setGridVisible(true);
  m_fromDate->setSelectedDate(d);
  layout->addWidget(m_fromDate, 1, 0);

  m_toDate = new QCalendarWidget(this);
  m_toDate->setGridVisible(true);
  d.setDate(d.year(), d.month(), d.daysInMonth());
  m_toDate->setSelectedDate(d);
  layout->addWidget(m_toDate, 1, 1);

  setLayout(layout);
}

QLabel *DateRangeWidget::setLabel(const QString &txt) {
  QLabel *lb = new QLabel(txt, this);
  lb->setAlignment(Qt::AlignCenter);
  return lb;
}

const QPair<QDate, QDate> DateRangeWidget::getDateRange() {
  QPair<QDate, QDate> p;
  p.first = m_fromDate->selectedDate();
  p.second = m_toDate->selectedDate();
  return p;
}

const QPair<QString, QString> DateRangeWidget::timestampRange() {
  QPair<QString, QString> p;
  QDateTime from(m_fromDate->selectedDate(), QTime(0, 0, 0));
  p.first = from.toString(ANTIQUA_DATETIME_FORMAT);
  QDateTime to(m_toDate->selectedDate(), QTime(23, 59, 0));
  p.second = to.toString(ANTIQUA_DATETIME_FORMAT);
  return p;
}
