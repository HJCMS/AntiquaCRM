// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "keywordactions.h"

#include <QHBoxLayout>
#include <QTimer>

KeywordActions::KeywordActions(QWidget *parent) : QFrame{parent} {
  QHBoxLayout *layout = new QHBoxLayout(this);

  m_messages = new QLabel(this);
  layout->addWidget(m_messages);

  layout->addStretch(1);

  m_btnQuit = new QPushButton(this);
  m_btnQuit->setText(tr("Close"));
  m_btnQuit->setIcon(QIcon(":/icons/action_quit.png"));
  layout->addWidget(m_btnQuit);

  setLayout(layout);

  connect(m_btnQuit, SIGNAL(clicked()), this, SIGNAL(sendQuit()));
}

void KeywordActions::statusMessage(const QString &msg) {
  m_messages->setText(msg);
  QTimer::singleShot(6000, m_messages, SLOT(clear()));
}
