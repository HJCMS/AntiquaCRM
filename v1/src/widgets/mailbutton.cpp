// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "mailbutton.h"

#include <QDebug>
#include <QIcon>
#include <QMenu>

MailButton::MailButton(QWidget *parent) : QPushButton{parent} {
  QIcon mailIcon = QIcon(":icons/user_identity.png");
  setIcon(mailIcon);
  setText(tr("Mail Messages"));
  setToolTip(tr("Selection for different eMail messages."));
  setEnabled(false);
  setVisible(false);

  QMenu *m_menu = new QMenu(this);
  // Einfache E-Mail Nachricht an Kunde.
  QString message_info = tr("Simple eMail message to customer.");
  ac_message = m_menu->addAction(mailIcon, message_info);
  ac_message->setWhatsThis(message_info);
  connect(ac_message, SIGNAL(triggered()), this, SLOT(setSimpleMail()));

  // Bestellstatus an den Kunden senden.
  QString status_info = tr("Send order status to customer.");
  ac_status = m_menu->addAction(mailIcon, status_info);
  ac_status->setWhatsThis(status_info);
  ac_status->setEnabled(false);
  ac_status->setVisible(false);
  connect(ac_status, SIGNAL(triggered()), this, SLOT(setStatusMail()));

  // Rechnung an den Käufer erstellen.
  QString invoice_info = tr("Create invoice to customer.");
  ac_invoice = m_menu->addAction(mailIcon, invoice_info);
  ac_invoice->setWhatsThis(invoice_info);
  ac_invoice->setEnabled(false);
  ac_invoice->setVisible(false);
  connect(ac_invoice, SIGNAL(triggered()), this, SLOT(setInvoceMail()));

  // Mitteilung Artikel Bestellung Storniert.
  QString cancel_info = tr("Message Item Order Cancelled.");
  ac_canceld = m_menu->addAction(mailIcon, cancel_info);
  ac_canceld->setWhatsThis(cancel_info);
  ac_canceld->setEnabled(false);
  ac_canceld->setVisible(false);
  connect(ac_canceld, SIGNAL(triggered()), this, SLOT(setCancelMail()));

  setMenu(m_menu);
}

void MailButton::setSimpleMail() {
  emit sendMailAction("MAIL_ACTION_SIMPLE_MESSAGE");
}

void MailButton::setStatusMail() {
  emit sendMailAction("MAIL_ACTION_SHIPPING_NOTICE");
}

void MailButton::setInvoceMail() {
  emit sendMailAction("MAIL_ACTION_INVOICE_BILLING");
}

void MailButton::setCancelMail() {
  emit sendMailAction("MAIL_ACTION_ORDER_CANCELED");
}

void MailButton::hasMailAddress(bool b) {
  setEnabled(b);
  setVisible(b);
}

void MailButton::setSections(MailButton::Sections flags) {
  sections = flags;
  // Alles zurücksetzen
  QList<QAction *> list = findChildren<QAction *>(QString());
  for (int c = 0; c < list.count(); c++) {
    QAction *ac = list.at(c);
    ac->setEnabled(false);
    ac->setVisible(false);
  }
  list.clear();

  // Wenn gesetzt, hier Aussteigen.
  if (flags.testFlag(NoButton)) {
    setEnabled(false);
    return;
  }

  // Auftrags-Nachrichten
  if (flags.testFlag(Orders)) {
    ac_invoice->setEnabled(true);
    ac_invoice->setVisible(true);
    ac_status->setEnabled(true);
    ac_status->setVisible(true);
    ac_canceld->setEnabled(true);
    ac_canceld->setVisible(true);
  }

  // Adressbuch-Nachrichten
  if (flags.testFlag(Customers)) {
    ac_message->setEnabled(true);
    ac_message->setVisible(true);
  }

  emit sendSectionChanged();
}

MailButton::Sections MailButton::getSections() { return sections; }
