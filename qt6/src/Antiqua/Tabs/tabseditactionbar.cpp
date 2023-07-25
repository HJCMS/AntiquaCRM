// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabseditactionbar.h"
#include "antiquaicon.h"

#include <QLayout>

namespace AntiquaCRM {

TabsEditActionBar::TabsEditActionBar(QWidget *parent) : QWidget{parent} {
  QHBoxLayout *layout = new QHBoxLayout(this);

  m_cancelBtn = new QPushButton(tr("Cancel"), this);
  m_cancelBtn->setObjectName("editor_action_button_cancel");
  m_cancelBtn->setIcon(AntiquaCRM::antiquaIcon("dialog-cancel"));
  m_cancelBtn->setShortcut(QKeySequence::Cancel);
  QString sc_cancel = m_cancelBtn->shortcut().toString();
  m_cancelBtn->setToolTip(tr("Exit without saving.") + " " + sc_cancel);
  m_cancelBtn->setWhatsThis(m_cancelBtn->toolTip());
  connect(m_cancelBtn, SIGNAL(clicked()), SIGNAL(sendCancelClicked()));
  layout->addWidget(m_cancelBtn);

  m_restoreBtn = new QPushButton(tr("Restore"), this);
  m_restoreBtn->setObjectName("editor_action_button_restore");
  m_restoreBtn->setIcon(AntiquaCRM::antiquaIcon("action-restore"));
  m_restoreBtn->setShortcut(QKeySequence::Undo);
  QString sc_undo = tr("Restore dataset to last save operation.");
  sc_undo.append(" " + m_restoreBtn->shortcut().toString());
  m_restoreBtn->setToolTip(sc_undo);
  m_restoreBtn->setWhatsThis(m_restoreBtn->toolTip());
  m_restoreBtn->setEnabled(false);
  connect(m_restoreBtn, SIGNAL(clicked()), SIGNAL(sendRestoreClicked()));
  layout->addWidget(m_restoreBtn);

  m_mailButton = new MailButton(this);
  m_mailButton->setObjectName("editor_action_mailer_button");
  layout->addWidget(m_mailButton);

  m_printerButton = new PrinterButton(this);
  m_printerButton->setObjectName("editor_action_print_button");
  layout->addWidget(m_printerButton);

  layout->addStretch(1);

  m_addArticle = new QPushButton(tr("Add Article"), this);
  m_addArticle->setObjectName("editor_action_button_article");
  m_addArticle->setIcon(AntiquaCRM::antiquaIcon("action-add"));
  m_addArticle->setToolTip(tr("Add a new Article."));
  m_addArticle->setWhatsThis(m_addArticle->toolTip());
  m_addArticle->setEnabled(false);
  m_addArticle->setVisible(false);
  connect(m_addArticle, SIGNAL(clicked()), SIGNAL(sendAddCustomAction()));
  layout->addWidget(m_addArticle);

  m_saveBtn = new QPushButton(tr("Save"), this);
  m_saveBtn->setObjectName("editor_action_button_save");
  m_saveBtn->setIcon(AntiquaCRM::antiquaIcon("action-save"));
  m_saveBtn->setShortcut(QKeySequence::Save);
  QString sc_save = m_saveBtn->shortcut().toString();
  m_saveBtn->setToolTip(tr("Save current dataset") + " " + sc_save);
  m_saveBtn->setWhatsThis(m_saveBtn->toolTip());
  connect(m_saveBtn, SIGNAL(clicked()), SIGNAL(sendSaveClicked()));
  layout->addWidget(m_saveBtn);

  m_readyBtn = new QPushButton(tr("Back to Mainview"), this);
  m_readyBtn->setObjectName("editor_action_button_back");
  m_readyBtn->setIcon(AntiquaCRM::antiquaIcon("action-undo"));
  m_readyBtn->setShortcut(QKeySequence::Back);
  QString sc_ready = m_readyBtn->shortcut().toString();
  m_readyBtn->setToolTip(tr("Go back to Mainview") + " " + sc_ready);
  m_readyBtn->setWhatsThis(m_readyBtn->toolTip());
  connect(m_readyBtn, SIGNAL(clicked()), SIGNAL(sendFinishClicked()));
  layout->addWidget(m_readyBtn);

  setLayout(layout);

  // Signals::PrinterButton
  connect(m_printerButton, SIGNAL(sendPrintDelivery()),
          SIGNAL(sendPrintDeliveryNote()));
  connect(m_printerButton, SIGNAL(sendPrintInvoice()),
          SIGNAL(sendPrintInvoiceNote()));
  connect(m_printerButton, SIGNAL(sendPaymentReminder()),
          SIGNAL(sendPrintPaymentReminder()));
  connect(m_printerButton, SIGNAL(sendPrintBookcard()),
          SIGNAL(sendPrintBookCard()));
  // Signals:MailButton
  connect(m_mailButton, SIGNAL(sendMailAction(const QString &)),
          SIGNAL(sendCreateMailMessage(const QString &)));
}

void TabsEditActionBar::setRestoreable(bool b) { m_restoreBtn->setEnabled(b); }

void TabsEditActionBar::setViewPrintButton(bool view) {
  m_printerButton->setEnabled(view);
  m_printerButton->setVisible(view);
}

void TabsEditActionBar::setPrinterMenu(AntiquaCRM::PrinterGroups buttons) {
  m_printerButton->setButtons(buttons);
}

void TabsEditActionBar::setViewMailButton(bool view) {
  m_mailButton->setEnabled(view);
  m_mailButton->setVisible(view);
}

void TabsEditActionBar::setMailMenu(AntiquaCRM::MailGroups group) {
  m_mailButton->setSections(group);
}

void TabsEditActionBar::setViewActionAddButton(bool view,
                                               const QString &title) {
  if (!title.isEmpty()) {
    m_addArticle->setText(title);
    m_addArticle->setToolTip(title);
  }

  m_addArticle->setEnabled(view);
  m_addArticle->setVisible(view);
}

void TabsEditActionBar::setViewRestoreButton(bool view) {
  m_restoreBtn->setEnabled(view);
  m_restoreBtn->setVisible(view);
}

bool TabsEditActionBar::isRestoreable() { return m_restoreBtn->isEnabled(); }

} // namespace AntiquaCRM
