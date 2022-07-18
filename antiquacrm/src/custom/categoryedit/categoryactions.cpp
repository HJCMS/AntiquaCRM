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
  m_btnVisible->setText(tr("Switch view"));
  m_btnVisible->setToolTip(tr("Hide and Show unused categories"));
  m_btnVisible->setIcon(myIcon("view_detailed"));
  layout->addWidget(m_btnVisible);

  m_search = new QLineEdit(this);
  m_search->setPlaceholderText(tr("Search Keyword"));
  m_search->setToolTip(tr("Add Keyword and Press Enter"));
  m_search->setMinimumWidth(150);
  layout->addWidget(m_search, Qt::AlignLeft);

  layout->addStretch(1);

  m_btnSave = new QPushButton(this);
  m_btnSave->setText(tr("Save"));
  m_btnSave->setIcon(myIcon("filesave"));
  m_btnSave->setShortcut(QKeySequence::Save);
  layout->addWidget(m_btnSave);

  m_btnQuit = new QPushButton(this);
  m_btnQuit->setText(tr("Close"));
  m_btnQuit->setIcon(myIcon("exit"));
  m_btnQuit->setShortcut(QKeySequence::Close);
  layout->addWidget(m_btnQuit);

  setLayout(layout);
  connect(m_btnToggle, SIGNAL(clicked()), this, SIGNAL(sendViewTree()));
  connect(m_btnVisible, SIGNAL(clicked()), this, SIGNAL(sendVisible()));
  connect(m_btnSave, SIGNAL(clicked()), this, SIGNAL(sendSaveDialog()));
  connect(m_btnQuit, SIGNAL(clicked()), this, SIGNAL(sendQuitDialog()));
  connect(m_search, SIGNAL(returnPressed()), this, SLOT(searchEnterPressed()));
}

void CategoryActions::searchEnterPressed() {
  QString txt = m_search->text().trimmed();
  txt.replace(",", "");
  txt.replace("/", "");
  emit sendSearch(txt);
}
