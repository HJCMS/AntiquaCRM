// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "editoractionbar.h"
#include "antiqua_global.h"
#include "myicontheme.h"

#include <QDebug>
#include <QHBoxLayout>

EditorActionBar::EditorActionBar(QWidget *parent) : QWidget{parent} {
  setObjectName("EditorActionBar");

  QHBoxLayout *layout = new QHBoxLayout(this);

  m_cancelBtn = new QPushButton(tr("Cancel"), this);
  m_cancelBtn->setObjectName("editor_action_button_cancle");
  m_cancelBtn->setToolTip(tr("Exit without saving."));
  m_cancelBtn->setIcon(myIcon("button_cancel"));
  connect(m_cancelBtn, SIGNAL(clicked()), this, SIGNAL(s_cancelClicked()));
  layout->addWidget(m_cancelBtn);

  m_restoreBtn = new QPushButton(tr("Restore"), this);
  m_restoreBtn->setObjectName("editor_action_button_restore");
  m_restoreBtn->setToolTip(tr("Restore dataset to last save operation."));
  m_restoreBtn->setIcon(myIcon("rebuild"));
  m_restoreBtn->setShortcut(QKeySequence::Undo);
  m_restoreBtn->setEnabled(false);
  connect(m_restoreBtn, SIGNAL(clicked()), this, SIGNAL(s_restoreClicked()));
  layout->addWidget(m_restoreBtn);

  m_printDeliveryBtn = new QPushButton(tr("Print Deliverynote"), this);
  m_printDeliveryBtn->setObjectName("editor_action_btn_print_delivery");
  m_printDeliveryBtn->setToolTip(tr("Open Print Deliverynote"));
  m_printDeliveryBtn->setIcon(myIcon("printer"));
  m_printDeliveryBtn->setShortcut(QKeySequence::Print);
  m_printDeliveryBtn->setEnabled(false);
  m_printDeliveryBtn->setVisible(false);
  connect(m_printDeliveryBtn, SIGNAL(clicked()), this,
          SIGNAL(s_printDeliveryNote()));
  layout->addWidget(m_printDeliveryBtn);

  m_printInvoiceBtn = new QPushButton(tr("Print Invoice"), this);
  m_printInvoiceBtn->setObjectName("editor_action_btn_print_invoice");
  m_printInvoiceBtn->setToolTip(tr("Open Print Invoice"));
  m_printInvoiceBtn->setIcon(myIcon("printer"));
  m_printInvoiceBtn->setEnabled(false);
  m_printInvoiceBtn->setVisible(false);
  connect(m_printInvoiceBtn, SIGNAL(clicked()), this,
          SIGNAL(s_printInvoiceNote()));
  layout->addWidget(m_printInvoiceBtn);

  layout->addStretch(1);

  m_saveBtn = new QPushButton(tr("Save"), this);
  m_saveBtn->setObjectName("editor_action_button_save");
  m_saveBtn->setToolTip(tr("Save current dataset"));
  m_saveBtn->setIcon(myIcon("filesave"));
  m_saveBtn->setShortcut(QKeySequence::Save);
  connect(m_saveBtn, SIGNAL(clicked()), this, SIGNAL(s_saveClicked()));
  layout->addWidget(m_saveBtn);

  m_readyBtn = new QPushButton(tr("Back"), this);
  m_readyBtn->setObjectName("editor_action_button_back");
  m_readyBtn->setToolTip(tr("Go back to Mainview"));
  m_readyBtn->setIcon(myIcon("button_ok"));
  connect(m_readyBtn, SIGNAL(clicked()), this, SIGNAL(s_finishClicked()));
  layout->addWidget(m_readyBtn);

  setLayout(layout);
}

void EditorActionBar::setRestoreable(bool b) { m_restoreBtn->setEnabled(b); }

void EditorActionBar::viewPrintButton(bool b) {
  m_printDeliveryBtn->setEnabled(b);
  m_printDeliveryBtn->setVisible(b);
  m_printInvoiceBtn->setEnabled(b);
  m_printInvoiceBtn->setVisible(b);
}

bool EditorActionBar::isRestoreable() { return m_restoreBtn->isEnabled(); }
