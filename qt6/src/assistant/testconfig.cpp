// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "testconfig.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

TestConfig::TestConfig(QWidget *parent) : QWidget{parent} {
  QVBoxLayout *layout = new QVBoxLayout(this);

  QLabel *t_info = new QLabel(this);
  t_info->setText("<h3>" + tr("Connection test") + "</h3>");
  layout->addWidget(t_info);

  m_info = new QTextEdit(this);
  layout->addWidget(m_info);

  QPushButton *btn = new QPushButton(tr("Connect"), this);
  btn->setStatusTip(tr("create connection test"));
  layout->addWidget(btn);

  setLayout(layout);

  connect(btn, SIGNAL(clicked()), this, SIGNAL(startTest()));
}

void TestConfig::setBodyText(const QString &str) { m_info->setPlainText(str); }
