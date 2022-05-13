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

  m_printBtn = new QPushButton(tr("Printing"), this);
  m_printBtn->setObjectName("editor_action_button_print");
  m_printBtn->setToolTip(tr("Open Print dialog"));
  m_printBtn->setIcon(myIcon("printer"));
  m_printBtn->setShortcut(QKeySequence::Print);
  m_printBtn->setEnabled(false);
  m_printBtn->setVisible(false);
  connect(m_printBtn, SIGNAL(clicked()), this, SIGNAL(s_printClicked()));
  layout->addWidget(m_printBtn);

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
  m_printBtn->setEnabled(b);
  m_printBtn->setVisible(b);
}

bool EditorActionBar::isRestoreable() { return m_restoreBtn->isEnabled(); }
