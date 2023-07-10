// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printerdevices.h"
#include "awhatsthisbutton.h"

#include <QLayout>
#include <QPrinterInfo>

PrinterDevices::PrinterDevices(QWidget *parent) : QGroupBox{parent} {
  setTitle(tr("Settings Default Printers"));

  QGridLayout *layout = new QGridLayout(this);
  layout->setColumnStretch(1, 1);

  layout->addWidget(label(tr("Primary")), 0, 0, 1, 1);
  m_mainPrinter = new AntiquaCRM::AComboBox(this);
  layout->addWidget(m_mainPrinter, 0, 1, 1, 1);
  QString infoMain = tr("Main printer for Invoices and Delivery notes.");
  layout->addWidget(new AntiquaCRM::AWhatsThisButton(infoMain, this), 0, 2, 1,
                    1);

  layout->addWidget(label(tr("Secondary")), 1, 0, 1, 1);
  m_slavePrinter = new AntiquaCRM::AComboBox(this);
  layout->addWidget(m_slavePrinter, 1, 1, 1, 1);
  QString infoSlave = tr("Secondary printer for Bookcards.");
  layout->addWidget(new AntiquaCRM::AWhatsThisButton(infoSlave, this), 1, 2, 1,
                    1);

  setLayout(layout);

  connect(m_mainPrinter, SIGNAL(currentIndexChanged(int)),
          SLOT(setDeviceChanged(int)));
  connect(m_slavePrinter, SIGNAL(currentIndexChanged(int)),
          SLOT(setDeviceChanged(int)));
}

void PrinterDevices::setDeviceChanged(int) { setWindowModified(true); }

QLabel *PrinterDevices::label(const QString &title) {
  QLabel *m_lb = new QLabel(this);
  m_lb->setIndent(2);
  m_lb->setText(title + ":");
  m_lb->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
  return m_lb;
}

const QIcon PrinterDevices::icon() const {
  return AntiquaCRM::antiquaIcon("printer");
}

void PrinterDevices::loadSection(AntiquaCRM::ASettings *config) {
  static const QRegularExpression strip("[_\\-]+");

  m_mainPrinter->clear();
  m_mainPrinter->setWithoutDisclosures();

  m_slavePrinter->clear();
  m_slavePrinter->setWithoutDisclosures();

  QStringList printers = QPrinterInfo::availablePrinterNames();
  if (printers.size() < 1) {
    qWarning("No Printer devices found!");
    return;
  }

  QStringListIterator it(printers);
  while (it.hasNext()) {
    QString name = it.next();
    QString display = name.replace(strip, " ");
    m_mainPrinter->addItem(icon(), display, name);
    m_slavePrinter->addItem(icon(), display, name);
  }

  QString primaryDevice = config->value("device_primary").toString();
  int i1 = m_mainPrinter->findData(primaryDevice, Qt::UserRole,
                                   Qt::MatchFixedString);
  if (i1 > 0) {
    m_mainPrinter->setCurrentIndex(i1);
  }

  QString secondaryDevice = config->value("device_secondary").toString();
  int i2 = m_slavePrinter->findData(secondaryDevice, Qt::UserRole,
                                    Qt::MatchFixedString);
  if (i2 > 0) {
    m_slavePrinter->setCurrentIndex(i2);
  }
}

void PrinterDevices::saveSection(AntiquaCRM::ASettings *config) {
  int _index = m_mainPrinter->currentIndex();
  if (_index > 0) {
    QVariant _value = m_mainPrinter->itemData(_index, Qt::UserRole);
    config->setValue("device_primary", _value.toString());
  }
  _index = m_slavePrinter->currentIndex();
  if (_index > 0) {
    QVariant _value = m_slavePrinter->itemData(_index, Qt::UserRole);
    config->setValue("device_secondary", _value.toString());
  }
}
