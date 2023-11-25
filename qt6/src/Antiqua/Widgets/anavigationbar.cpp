// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "anavigationbar.h"

#include <QLayout>
#include <QStyle>

namespace AntiquaCRM {

ANavigationBar::ANavigationBar(QWidget *parent, int count)
    : QWidget{parent}, p_count{count - 1}, p_index{0} {
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

void ANavigationBar::setCurrentIndex(int index) {
  if (index < 0 || index > p_count)
    return;

  p_index = index;
}

void ANavigationBar::setAllowPrev(int index) {
  if (index > 0)
    btn_back->setEnabled(true);
  else
    btn_back->setEnabled(false);
}

void ANavigationBar::setAllowNext(int index, bool status) {
  if (index == 0 || index < p_count) {
    btn_next->setEnabled(status);
    return;
  }
  btn_next->setEnabled(false);
}

int ANavigationBar::getCurrentIndex() { return p_index; }

} // namespace AntiquaCRM
