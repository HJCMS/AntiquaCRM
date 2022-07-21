// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "keywordactions.h"
#include "myicontheme.h"

#include <QHBoxLayout>
#include <QStyle>

KeywordActions::KeywordActions(QWidget *parent) : QFrame{parent} {
  QHBoxLayout *layout = new QHBoxLayout(this);

  QStyle *m_style = style();

  m_btnReload = new QPushButton(this);
  m_btnReload->setText(tr("Restore"));
  m_btnReload->setIcon(myIcon("reload"));
  layout->addWidget(m_btnReload);

  layout->addStretch(1);

  m_btnQuit = new QPushButton(this);
  m_btnQuit->setText(tr("Quit"));
  m_btnQuit->setIcon(myIcon("exit"));
  layout->addWidget(m_btnQuit);

  setLayout(layout);

  connect(m_btnReload, SIGNAL(clicked()), this, SIGNAL(sendReload()));
  connect(m_btnQuit, SIGNAL(clicked()), this, SIGNAL(sendQuit()));
}
