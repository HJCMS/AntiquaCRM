// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printerbutton.h"
#include "myicontheme.h"

#include <QDebug>
#include <QMenu>

PrinterButton::PrinterButton(QWidget *parent, PrinterButton::Buttons flags)
    : QPushButton{parent} {
  QIcon printIcon = myIcon("printer");
  setIcon(printIcon);
  setText(tr("Printing"));
  setEnabled(false);

  QMenu *m_menu = new QMenu(this);

  ac_delivery = m_menu->addAction(printIcon, tr("Deliverynote"));
  ac_delivery->setToolTip(tr("Open Print Deliverynote"));
  ac_delivery->setEnabled(false);
  ac_delivery->setVisible(false);
  connect(ac_delivery, SIGNAL(triggered()), this, SIGNAL(sendPrintDelivery()));

  ac_invoice = m_menu->addAction(printIcon, tr("Invoice"));
  ac_invoice->setToolTip(tr("Open Print Invoice"));
  ac_invoice->setEnabled(false);
  ac_invoice->setVisible(false);
  connect(ac_invoice, SIGNAL(triggered()), this, SIGNAL(sendPrintInvoice()));

  ac_reminder = m_menu->addAction(printIcon, tr("Payment Reminder"));
  ac_reminder->setToolTip(tr("Open Print Invoice"));
  ac_reminder->setEnabled(false);
  ac_reminder->setVisible(false);
  connect(ac_reminder, SIGNAL(triggered()), this,
          SIGNAL(sendPaymentReminder()));

  ac_bookcard = m_menu->addAction(printIcon, tr("Book card"));
  ac_bookcard->setToolTip(tr("Printing a Book card"));
  ac_bookcard->setEnabled(false);
  ac_bookcard->setVisible(false);
  connect(ac_bookcard, SIGNAL(triggered()), this, SIGNAL(sendPrintBookcard()));

  setMenu(m_menu);
  setButtons(flags);
}

void PrinterButton::setButtons(PrinterButton::Buttons flags) {
  buttons = flags;
  if (flags.testFlag(NoButton))
    return;

  if (flags.testFlag(Delivery)) {
    ac_delivery->setEnabled(true);
    ac_delivery->setVisible(true);
    setEnabled(true);
  }

  if (flags.testFlag(Invoice)) {
    ac_invoice->setEnabled(true);
    ac_invoice->setVisible(true);
    setEnabled(true);
  }

  if (flags.testFlag(Reminder)) {
    ac_reminder->setEnabled(true);
    ac_reminder->setVisible(true);
    setEnabled(true);
  }

  if (flags.testFlag(Bookcard)) {
    ac_bookcard->setEnabled(true);
    ac_bookcard->setVisible(true);
    setEnabled(true);
  }
  emit buttonsChanged();
}

PrinterButton::Buttons PrinterButton::getButtons() {
  return buttons;
}
