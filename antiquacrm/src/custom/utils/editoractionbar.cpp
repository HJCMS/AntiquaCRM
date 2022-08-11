// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "editoractionbar.h"
#include "myicontheme.h"

#include <QDebug>
#include <QHBoxLayout>

EditorActionBar::EditorActionBar(QWidget *parent) : QWidget{parent} {
  setObjectName("EditorActionBar");

  QHBoxLayout *layout = new QHBoxLayout(this);

  m_cancelBtn = new QPushButton(tr("Cancel"), this);
  m_cancelBtn->setObjectName("editor_action_button_cancle");
  m_cancelBtn->setIcon(myIcon("button_cancel"));
  m_cancelBtn->setShortcut(QKeySequence::Cancel);
  QString sc_cancel = m_cancelBtn->shortcut().toString();
  m_cancelBtn->setToolTip(tr("Exit without saving.") + " " + sc_cancel);
  connect(m_cancelBtn, SIGNAL(clicked()), this, SIGNAL(s_cancelClicked()));
  layout->addWidget(m_cancelBtn);

  m_restoreBtn = new QPushButton(tr("Restore"), this);
  m_restoreBtn->setObjectName("editor_action_button_restore");
  m_restoreBtn->setIcon(myIcon("rebuild"));
  m_restoreBtn->setShortcut(QKeySequence::Undo);
  QString sc_undo = tr("Restore dataset to last save operation.");
  sc_undo.append(" " + m_restoreBtn->shortcut().toString());
  m_restoreBtn->setToolTip(sc_undo);
  m_restoreBtn->setEnabled(false);
  connect(m_restoreBtn, SIGNAL(clicked()), this, SIGNAL(s_restoreClicked()));
  layout->addWidget(m_restoreBtn);

  m_printerButton = new PrinterButton(this);
  m_printerButton->setObjectName("editor_action_print_button");
  layout->addWidget(m_printerButton);

  m_mailButton = new MailButton(this);
  m_mailButton->setObjectName("editor_action_mailer_button");
  layout->addWidget(m_mailButton);
  // Standard ausblenden
  setViewMailButton(false);

  layout->addStretch(1);

  m_saveBtn = new QPushButton(tr("Save"), this);
  m_saveBtn->setObjectName("editor_action_button_save");
  m_saveBtn->setIcon(myIcon("filesave"));
  m_saveBtn->setShortcut(QKeySequence::Save);
  QString sc_save = m_saveBtn->shortcut().toString();
  m_saveBtn->setToolTip(tr("Save current dataset") + " " + sc_save);
  connect(m_saveBtn, SIGNAL(clicked()), this, SIGNAL(s_saveClicked()));
  layout->addWidget(m_saveBtn);

  m_readyBtn = new QPushButton(tr("Back to Mainview"), this);
  m_readyBtn->setObjectName("editor_action_button_back");
  m_readyBtn->setIcon(myIcon("button_ok"));
  m_readyBtn->setShortcut(QKeySequence::PreviousChild);
  QString sc_ready = m_readyBtn->shortcut().toString();
  m_readyBtn->setToolTip(tr("Go back to Mainview") + " " + sc_ready);
  connect(m_readyBtn, SIGNAL(clicked()), this, SIGNAL(s_finishClicked()));
  layout->addWidget(m_readyBtn);

  setLayout(layout);

  connect(m_printerButton, SIGNAL(sendPrintDelivery()), this,
          SIGNAL(s_printDeliveryNote()));
  connect(m_printerButton, SIGNAL(sendPrintInvoice()), this,
          SIGNAL(s_printInvoiceNote()));
  connect(m_printerButton, SIGNAL(sendPaymentReminder()), this,
          SIGNAL(s_printPaymentReminder()));
  connect(m_printerButton, SIGNAL(sendPrintBookcard()), this,
          SIGNAL(s_printBookCard()));
}

void EditorActionBar::setRestoreable(bool b) {
  m_restoreBtn->setEnabled(b);
}

void EditorActionBar::setViewPrintButton(bool b) {
  m_printerButton->setEnabled(b);
  m_printerButton->setVisible(b);
}

void EditorActionBar::setPrinterMenu(PrinterButton::Buttons buttons) {
  m_printerButton->setButtons(buttons);
}

void EditorActionBar::setViewMailButton(bool b) {
  m_mailButton->setEnabled(b);
  m_mailButton->setVisible(b);
}

void EditorActionBar::setMailMenu(/* TODO */) {}

void EditorActionBar::setViewRestoreButton(bool b) {
  m_restoreBtn->setEnabled(b);
  m_restoreBtn->setVisible(b);
}

bool EditorActionBar::isRestoreable() { return m_restoreBtn->isEnabled(); }
