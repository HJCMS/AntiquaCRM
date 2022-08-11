// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "mailbutton.h"
#include "myicontheme.h"

#include <QDebug>
#include <QMenu>

MailButton::MailButton(QWidget *parent, MailButton::Buttons flags)
    : QPushButton{parent} {
  QIcon mailIcon = myIcon("identity");
  setIcon(mailIcon);
  setText(tr("eMail"));
  setEnabled(false);

  QMenu *m_menu = new QMenu(this);

  ac_invoice = m_menu->addAction(mailIcon, tr("Invoice"));
  ac_invoice->setToolTip(tr("Invoice eMail"));
  ac_invoice->setEnabled(false);
  ac_invoice->setVisible(false);
  connect(ac_invoice, SIGNAL(triggered()), this, SIGNAL(sendMailInvoce()));

  ac_simple = m_menu->addAction(mailIcon, tr("eMail"));
  ac_simple->setEnabled(false);
  ac_simple->setVisible(false);
  connect(ac_simple, SIGNAL(triggered()), this, SIGNAL(sendMailSimple()));

  setMenu(m_menu);
  setButtons(flags);
}

void MailButton::setButtons(MailButton::Buttons flags) {
  buttons = flags;
  if (flags.testFlag(NoButton))
    return;

  if (flags.testFlag(Invoice)) {
    ac_invoice->setEnabled(true);
    ac_invoice->setVisible(true);
    setEnabled(true);
  }

  if (flags.testFlag(Simple)) {
    ac_simple->setEnabled(true);
    ac_simple->setVisible(true);
    setEnabled(true);
  }

  emit buttonsChanged();
}

MailButton::Buttons MailButton::getButtons() { return buttons; }
