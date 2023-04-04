// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabseditactionbar.h"

#include <AntiquaInput>
#include <QLayout>

namespace AntiquaCRM {

TabsEditActionBar::TabsEditActionBar(QWidget *parent) : QWidget{parent} {

  QHBoxLayout *layout = new QHBoxLayout(this);

  m_cancelBtn = new QPushButton(tr("Cancel"), this);
  m_cancelBtn->setObjectName("editor_action_button_cancel");
  m_cancelBtn->setIcon(AntiquaApplIcon("action-cancel"));
  m_cancelBtn->setShortcut(QKeySequence::Cancel);
  QString sc_cancel = m_cancelBtn->shortcut().toString();
  m_cancelBtn->setToolTip(tr("Exit without saving.") + " " + sc_cancel);
  connect(m_cancelBtn, SIGNAL(clicked()), SIGNAL(sendCancelClicked()));
  layout->addWidget(m_cancelBtn);

  m_restoreBtn = new QPushButton(tr("Restore"), this);
  m_restoreBtn->setObjectName("editor_action_button_restore");
  m_restoreBtn->setIcon(AntiquaApplIcon("action-restore"));
  m_restoreBtn->setShortcut(QKeySequence::Undo);
  QString sc_undo = tr("Restore dataset to last save operation.");
  sc_undo.append(" " + m_restoreBtn->shortcut().toString());
  m_restoreBtn->setToolTip(sc_undo);
  m_restoreBtn->setEnabled(false);
  connect(m_restoreBtn, SIGNAL(clicked()), SIGNAL(sendRestoreClicked()));
  layout->addWidget(m_restoreBtn);

  m_mailButton = new QPushButton(this);
  m_mailButton->setObjectName("editor_action_mailer_button");
  layout->addWidget(m_mailButton);

  m_printerButton = new QPushButton(this);
  m_printerButton->setObjectName("editor_action_print_button");
  layout->addWidget(m_printerButton);

  layout->addStretch(1);

  m_addArticle = new QPushButton(tr("Add Article"), this);
  m_addArticle->setObjectName("editor_action_button_article");
  m_addArticle->setIcon(QIcon("://icons/action_add.png"));
  m_addArticle->setEnabled(false);
  m_addArticle->setVisible(false);
  connect(m_addArticle, SIGNAL(clicked()), SIGNAL(sendAddCustomAction()));
  layout->addWidget(m_addArticle);

  m_saveBtn = new QPushButton(tr("Save"), this);
  m_saveBtn->setObjectName("editor_action_button_save");
  m_saveBtn->setIcon(AntiquaApplIcon("action-save"));
  m_saveBtn->setShortcut(QKeySequence::Save);
  QString sc_save = m_saveBtn->shortcut().toString();
  m_saveBtn->setToolTip(tr("Save current dataset") + " " + sc_save);
  connect(m_saveBtn, SIGNAL(clicked()), SIGNAL(sendSaveClicked()));
  layout->addWidget(m_saveBtn);

  m_readyBtn = new QPushButton(tr("Back to Mainview"), this);
  m_readyBtn->setObjectName("editor_action_button_back");
  m_readyBtn->setIcon(AntiquaApplIcon("action-ok"));
  m_readyBtn->setShortcut(QKeySequence::Back);
  QString sc_ready = m_readyBtn->shortcut().toString();
  m_readyBtn->setToolTip(tr("Go back to Mainview") + " " + sc_ready);
  connect(m_readyBtn, SIGNAL(clicked()), SIGNAL(sendFinishClicked()));
  layout->addWidget(m_readyBtn);

  setLayout(layout);

  // Signals::Printer Button
  /*
  connect(m_printerButton, SIGNAL(sendPrintDelivery()),
          SIGNAL(sendPrintDeliveryNote()));
  connect(m_printerButton, SIGNAL(sendPrintInvoice()),
          SIGNAL(sendPrintInvoiceNote()));
  connect(m_printerButton, SIGNAL(sendPaymentReminder()),
          SIGNAL(sendPrintPaymentReminder()));
  connect(m_printerButton, SIGNAL(sendPrintBookcard()),
          SIGNAL(sendPrintBookCard()));
  // Signals:Mailer Button
  connect(m_mailButton, SIGNAL(sendMailAction(const QString &)),
          SIGNAL(sendCreateMailMessage(const QString &)));
  */
}

void TabsEditActionBar::setRestoreable(bool b) { m_restoreBtn->setEnabled(b); }

void TabsEditActionBar::setFocusButton(const QString &suffix) {
  QString search("editor_action_button_" + suffix.toLower());
  QPushButton *btn = findChild<QPushButton *>(search);
  if (btn != nullptr && btn->isVisible()) {
    btn->setFocus();
  }
}

void TabsEditActionBar::setViewPrintButton(bool b) {
  m_printerButton->setEnabled(b);
  m_printerButton->setVisible(b);
}

//void TabsEditActionBar::setPrinterMenu(PrinterButton::Buttons buttons) {
//  m_printerButton->setButtons(buttons);
//}

void TabsEditActionBar::setViewMailButton(bool) {
  //m_mailButton->hasMailAddress(b);
}

//void TabsEditActionBar::setMailMenu(MailButton::Sections sections) {
//  m_mailButton->setSections(sections);
//}

void TabsEditActionBar::setViewActionAddButton(bool b,
                                               const QString &customTitle) {
  if (!customTitle.isEmpty())
    m_addArticle->setText(customTitle);

  m_addArticle->setEnabled(b);
  m_addArticle->setVisible(b);
}

void TabsEditActionBar::setViewRestoreButton(bool b) {
  m_restoreBtn->setEnabled(b);
  m_restoreBtn->setVisible(b);
}

bool TabsEditActionBar::isRestoreable() { return m_restoreBtn->isEnabled(); }

} // namespace AntiquaCRM
