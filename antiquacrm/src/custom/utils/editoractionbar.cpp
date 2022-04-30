// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "editoractionbar.h"
#include "antiqua_global.h"
#include "myicontheme.h"

#include <QtCore/QDebug>
#include <QtWidgets/QHBoxLayout>

EditorActionBar::EditorActionBar(QWidget *parent) : QWidget{parent} {
  setObjectName("EditorActionBar");

  QHBoxLayout *layout = new QHBoxLayout(this);

  m_cancelBtn = new QPushButton(tr("Cancel"), this);
  m_cancelBtn->setObjectName("CancelEditorActionBar");
  m_cancelBtn->setToolTip(tr("Exit without saving."));
  m_cancelBtn->setIcon(myIcon("button_cancel"));
  layout->addWidget(m_cancelBtn);

  m_restoreBtn = new QPushButton(tr("Restore"), this);
  m_restoreBtn->setObjectName("RestoreEditorActionBar");
  m_restoreBtn->setToolTip(tr("Restore dataset to last save operation."));
  m_restoreBtn->setIcon(myIcon("rebuild"));
  m_restoreBtn->setShortcut(QKeySequence::Undo);
  m_restoreBtn->setEnabled(false);
  layout->addWidget(m_restoreBtn);

  layout->addStretch(1);

  m_saveBtn = new QPushButton(tr("Save"), this);
  m_saveBtn->setObjectName("SaveButtonDialog");
  m_saveBtn->setToolTip(tr("Save current dataset"));
  m_saveBtn->setIcon(myIcon("filesave"));
  m_saveBtn->setShortcut(QKeySequence::Save);
  layout->addWidget(m_saveBtn);

  m_readyBtn = new QPushButton(tr("Back"), this);
  m_readyBtn->setObjectName("FinishEditorActionBar");
  m_readyBtn->setToolTip(tr("Go back to Mainview"));
  m_readyBtn->setIcon(myIcon("button_ok"));
  layout->addWidget(m_readyBtn);

  setLayout(layout);

  connect(m_restoreBtn, SIGNAL(clicked()), this, SIGNAL(s_restoreClicked()));
  connect(m_cancelBtn, SIGNAL(clicked()), this, SIGNAL(s_cancelClicked()));
  connect(m_saveBtn, SIGNAL(clicked()), this, SIGNAL(s_saveClicked()));
  connect(m_readyBtn, SIGNAL(clicked()), this, SIGNAL(s_finishClicked()));
}

void EditorActionBar::setRestoreable(bool b) { m_restoreBtn->setEnabled(b); }

bool EditorActionBar::isRestoreable() { return m_restoreBtn->isEnabled(); }