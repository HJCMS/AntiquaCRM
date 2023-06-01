// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printerbutton.h"
#include "ainputwidget.h"

#include <QDebug>
#include <QIcon>
#include <QMenu>

namespace AntiquaCRM {

PrinterButton::PrinterButton(QWidget *parent, AntiquaCRM::PrinterGroups flags)
    : QPushButton{parent} {
  QIcon _icon = AntiquaApplIcon("printer");
  setIcon(_icon);
  setText(tr("Printing"));
  setEnabled(false);
  setToolTip(tr("Selection for different printings."));
  setWhatsThis(toolTip());

  QMenu *m_menu = new QMenu(this);

  ac_delivery = m_menu->addAction(_icon, tr("Deliverynote"));
  ac_delivery->setToolTip(tr("Open Print Deliverynote"));
  ac_delivery->setEnabled(false);
  ac_delivery->setVisible(false);
  connect(ac_delivery, SIGNAL(triggered()), this, SIGNAL(sendPrintDelivery()));

  ac_invoice = m_menu->addAction(_icon, tr("Invoice"));
  ac_invoice->setToolTip(tr("Open Print Invoice"));
  ac_invoice->setEnabled(false);
  ac_invoice->setVisible(false);
  connect(ac_invoice, SIGNAL(triggered()), this, SIGNAL(sendPrintInvoice()));

  ac_reminder = m_menu->addAction(_icon, tr("Payment Reminder"));
  ac_reminder->setToolTip(tr("Open Print Invoice"));
  ac_reminder->setEnabled(false);
  ac_reminder->setVisible(false);
  connect(ac_reminder, SIGNAL(triggered()), this,
          SIGNAL(sendPaymentReminder()));

  ac_bookcard = m_menu->addAction(_icon, tr("Book card"));
  ac_bookcard->setToolTip(tr("Printing a Book card"));
  ac_bookcard->setEnabled(false);
  ac_bookcard->setVisible(false);
  connect(ac_bookcard, SIGNAL(triggered()), this, SIGNAL(sendPrintBookcard()));

  setMenu(m_menu);
  setButtons(flags);
}

void PrinterButton::setButtons(AntiquaCRM::PrinterGroups flags) {
  settings = flags;
  if (flags.testFlag(PRINT_NOBUTTON))
    return;

  if (flags.testFlag(PRINT_DELIVERY)) {
    ac_delivery->setEnabled(true);
    ac_delivery->setVisible(true);
    setEnabled(true);
  }

  if (flags.testFlag(PRINT_INVOICE)) {
    ac_invoice->setEnabled(true);
    ac_invoice->setVisible(true);
    setEnabled(true);
  }

  if (flags.testFlag(PRINT_REMINDER)) {
    ac_reminder->setEnabled(true);
    ac_reminder->setVisible(true);
    setEnabled(true);
  }

  if (flags.testFlag(PRINT_BOOKCARD)) {
    ac_bookcard->setEnabled(true);
    ac_bookcard->setVisible(true);
    setEnabled(true);
  }
  emit sendButtonsChanged();
}

AntiquaCRM::PrinterGroups PrinterButton::getButtons() { return settings; }

} // namespace AntiquaCRM
