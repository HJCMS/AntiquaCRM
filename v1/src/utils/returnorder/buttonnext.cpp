// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "buttonnext.h"

#include <QHBoxLayout>
#include <QIcon>

ButtonNext::ButtonNext(QWidget *parent) : QFrame{parent} {
  setContentsMargins(5, 5, 5, 5);
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addStretch(1);
  m_btn = new QPushButton(tr("further"), this);
  m_btn->setIcon(QIcon("://icons/action_ok.png"));
  layout->addWidget(m_btn);
  layout->addStretch(1);
  setLayout(layout);
  connect(m_btn, SIGNAL(clicked()), SIGNAL(clicked()));
}
