// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "monthselecter.h"

#include <QCalendar>
#include <QIcon>
#include <QLabel>
#include <QLayout>
#include <QLocale>

MonthSelecter::MonthSelecter(QWidget *parent) : QStatusBar{parent} {
  setContentsMargins(0, 0, 0, 0);
  setSizeGripEnabled(false);

  m_comboBox = new AntiquaComboBox(this);
  m_comboBox->insertItem(0, tr("No month selected"), 0);
  createSelectionRange();
  insertPermanentWidget(0, m_comboBox);

  btn_startReport = new QPushButton(this);
  btn_startReport->setIcon(QIcon("://icons/spreadsheet.png"));
  btn_startReport->setText(tr("Create report"));
  insertPermanentWidget(1, btn_startReport);

  btn_printReport = new QPushButton(this);
  btn_printReport->setIcon(QIcon("://icons/printer.png"));
  btn_printReport->setText(tr("Print"));
  btn_printReport->setEnabled(false);
  insertPermanentWidget(2, btn_printReport);

  btn_saveReport = new QPushButton(this);
  btn_saveReport->setIcon(QIcon("://icons/action_saveas.png"));
  btn_saveReport->setText(tr("Save"));
  btn_saveReport->setEnabled(false);
  insertPermanentWidget(3, btn_saveReport);

  connect(btn_startReport, SIGNAL(clicked()), SLOT(createPressed()));
  connect(btn_printReport, SIGNAL(clicked()), SIGNAL(sendPrintReport()));
  connect(btn_saveReport, SIGNAL(clicked()), SIGNAL(sendSaveReport()));
}

void MonthSelecter::createSelectionRange() {
  // Kalender Übsersetzung
  QLocale locale = QLocale::system();
  // Aktuelles Datum
  QDate date = QDate::currentDate();
  // Aktueller Monat
  qint8 current_month = date.month();
  // Aktuelles Jahr
  int current_year = date.year();
  QString str_current_year = QString::number(current_year);
  // Letztes Jahr
  int past_year = (date.year() - 1);
  QString str_past_year = QString::number(past_year);
  // Monate in die Vergagenheit gehen!
  qint8 past_months = (current_month - 6);
  // Zähler für vergangenes Jahr
  qint8 minus = 0;
  // Sortierung für die Auswahlbox
  qint8 position = 1;
  // Initialisiere den Kalender
  QCalendar cal(QCalendar::System::Gregorian);
  // Starte Schleife in die Vergangeheit.
  for (qint8 x = current_month; x > past_months; x--) {
    if (x > 0) {
      qint8 day = cal.daysInMonth(x, current_year);
      QString name = cal.monthName(locale, x, current_year);
      name.append(" " + str_current_year);
      QDate dt = cal.dateFromParts(current_year, x, day);
      m_comboBox->insertItem(position, name, dt);
      if (dt.month() == (current_month - 1))
        m_comboBox->setCurrentIndex(position);

      position++;
    } else {
      qint8 past_month = (12 - minus++);
      qint8 day = cal.daysInMonth(x, current_year);
      QString name = cal.monthName(locale, past_month, past_year);
      name.append(" " + str_past_year);
      QDate dt = cal.dateFromParts(past_year, past_month, day);
      m_comboBox->insertItem(position, name, dt);
      position++;
    }
  }
}

void MonthSelecter::createPressed() {
  int index = m_comboBox->currentIndex();
  if (index > 0) {
    QDate d = m_comboBox->itemData(index, Qt::UserRole).toDate();
    btn_saveReport->setEnabled(true);
    emit sendMonthDate(d);
  } else {
    btn_saveReport->setEnabled(false);
  }
}

const QString MonthSelecter::selectedText() {
  return m_comboBox->currentText();
}

const QDate MonthSelecter::selectedMonth() {
  int index = m_comboBox->currentIndex();
  return m_comboBox->itemData(index, Qt::UserRole).toDate();
}
