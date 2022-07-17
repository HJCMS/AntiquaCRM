// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "categoryactions.h"
#include "myicontheme.h"

#include <QHBoxLayout>

CategoryActions::CategoryActions(QWidget *parent) : QFrame{parent} {

  QHBoxLayout *layout = new QHBoxLayout(this);

  m_btnToggle = new QPushButton(this);
  m_btnToggle->setText(tr("Treeview"));
  m_btnToggle->setToolTip(tr("Open/close Treeview"));
  m_btnToggle->setIcon(myIcon("view_choose"));
  layout->addWidget(m_btnToggle);

  m_btnVisible = new QPushButton(this);
  m_btnVisible->setText(tr("Hide/Show Unused"));
  m_btnVisible->setIcon(myIcon("view_detailed"));
  layout->addWidget(m_btnVisible);

  layout->addStretch(1);

  m_btnSave = new QPushButton(this);
  m_btnSave->setText(tr("Save"));
  m_btnSave->setIcon(myIcon("filesave"));
  layout->addWidget(m_btnSave);

  m_btnQuit = new QPushButton(this);
  m_btnQuit->setText(tr("Close"));
  m_btnQuit->setIcon(myIcon("exit"));
  layout->addWidget(m_btnQuit);

  setLayout(layout);
  connect(m_btnToggle, SIGNAL(clicked()), this, SIGNAL(sendViewTree()));
  connect(m_btnVisible, SIGNAL(clicked()), this, SIGNAL(sendVisible()));
  connect(m_btnSave, SIGNAL(clicked()), this, SIGNAL(sendSaveDialog()));
  connect(m_btnQuit, SIGNAL(clicked()), this, SIGNAL(sendQuitDialog()));
}
