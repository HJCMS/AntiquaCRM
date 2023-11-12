// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "anavigationbar.h"

#include <QLayout>
#include <QStyle>

namespace AntiquaCRM {

ANavigationBar::ANavigationBar(QWidget *parent, int count)
    : QWidget{parent}, p_count{count - 1} {
  QHBoxLayout *layout = new QHBoxLayout(this);
  btn_back = new QPushButton(tr("Back"), this);
  btn_back->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
  btn_back->setEnabled(false);
  layout->addWidget(btn_back);
  layout->addStretch(1);
  btn_next = new QPushButton(tr("Next"), this);
  btn_next->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
  btn_next->setEnabled(false);
  layout->addWidget(btn_next);
  setLayout(layout);
  connect(btn_back, SIGNAL(clicked()), SIGNAL(sendPrev()));
  connect(btn_next, SIGNAL(clicked()), SIGNAL(sendNext()));
}

void ANavigationBar::setIndex(int index) {
  switch (index) {
  case 0:
    btn_back->setEnabled(false);
    btn_next->setEnabled(true);
    break;

  case 1:
    btn_back->setEnabled(true);
    btn_next->setEnabled((p_count > 1));
    break;

  default:
    btn_back->setEnabled((index > 0));
    btn_next->setEnabled((p_count > index));
    break;
  }
}

} // namespace AntiquaCRM
