// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printersetup.h"

#include <QIcon>
#include <QLabel>
#include <QLayout>
#include <QPrinterInfo>
#include <QRegExp>

PrinterSetup::PrinterSetup(QWidget *parent) : QGroupBox{parent} {
  setTitle(tr("Settings Default Printers"));

  QIcon wIcon("://icons/warning.png");
  QString warn = tr("Without disclosures");
  QGridLayout *prLayout = new QGridLayout(this);

  // Primary Printer
  QString txt = tr("Primary") + " ";
  prLayout->addWidget(label(txt), 0, 0, 1, 1);

  m_mainPrinter = new QComboBox(this);
  m_mainPrinter->addItem(wIcon, warn, QString());
  prLayout->addWidget(m_mainPrinter, 0, 1, 1, 1);

  txt = tr("Main printer for Invoices and Delivery notes.");
  prLayout->addWidget(new QLabel(txt, this), 0, 2, 1, 1);

  // Secondary Printer
  txt = tr("Secondary") + " ";
  prLayout->addWidget(label(txt), 1, 0, 1, 1);

  m_slavePrinter = new QComboBox(this);
  m_slavePrinter->addItem(wIcon, warn, QString());
  prLayout->addWidget(m_slavePrinter, 1, 1, 1, 1);

  txt = tr("Secondary printer for Bookcards.");
  prLayout->addWidget(new QLabel(txt, this), 1, 2, 1, 1);

  prLayout->setColumnStretch(1, 1);
  setLayout(prLayout);
}

QLabel *PrinterSetup::label(const QString &title) {
  QLabel *lb = new QLabel(title + ":", this);
  lb->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  return lb;
}

void PrinterSetup::init(AntiquaCRM::ASettings *config) {
  // Verfügbare Drucker
  QRegExp strip("[_\\-]+");
  QIcon prIcon("://icons/printer.png");
  QListIterator<QPrinterInfo> it(QPrinterInfo::availablePrinters());
  while (it.hasNext()) {
    QPrinterInfo info = it.next();
    if (!info.isNull()) {
      QString name = info.printerName();
      QString display = info.printerName().replace(strip, " ");
      m_mainPrinter->addItem(prIcon, display, name);
      m_slavePrinter->addItem(prIcon, display, name);
    }
  }

  int i = 0;
  QString a4p = config->value("printer/DIN_A4_Printer").toString();
  i = m_mainPrinter->findData(a4p, Qt::UserRole, Qt::MatchFixedString);
  if (i > 0) {
    m_mainPrinter->setCurrentIndex(i);
  }

  QString a6p = config->value("printer/DIN_A6_Printer").toString();
  i = m_slavePrinter->findData(a6p, Qt::UserRole, Qt::MatchFixedString);
  if (i > 0) {
    m_slavePrinter->setCurrentIndex(i);
  }
}

const QString PrinterSetup::mainPrinter() {
  int i = m_mainPrinter->currentIndex();
  return m_mainPrinter->itemData(i, Qt::UserRole).toString();
}

const QString PrinterSetup::slavePrinter() {
  int i = m_slavePrinter->currentIndex();
  return m_slavePrinter->itemData(i, Qt::UserRole).toString();
}

const QPair<QString, QString> PrinterSetup::printers() {
  QPair<QString, QString> p(mainPrinter(), slavePrinter());
  return p;
}
