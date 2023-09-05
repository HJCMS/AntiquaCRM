// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "provideractionnavigator.h"

#include <AntiquaWidgets>
#include <QHBoxLayout>
#include <QSpacerItem>

namespace AntiquaCRM {

ProviderActionNavigator::ProviderActionNavigator(QStackedWidget *pager,
                                                 QWidget *parent)
    : QFrame{parent}, stackedWidget{pager} {
  setFrameStyle(QFrame::NoFrame);
  setContentsMargins(0, 0, 0, 0);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  btn_back = new QPushButton(tr("Previous"), this);
  btn_back->setObjectName("previous");
  btn_back->setIcon(AntiquaCRM::antiquaIcon("arrow-left"));
  btn_back->setEnabled(false);
  layout->addWidget(btn_back);

  layout->addStretch(1);

  btn_next = new QPushButton(tr("Forward"), this);
  btn_next->setObjectName("forward");
  btn_next->setIcon(AntiquaCRM::antiquaIcon("arrow-right"));
  layout->addWidget(btn_next);

  setLayout(layout);

  connect(btn_back, SIGNAL(clicked()), SLOT(prepareGotoSignal()));
  connect(btn_next, SIGNAL(clicked()), SLOT(prepareGotoSignal()));
  connect(stackedWidget, SIGNAL(currentChanged(int)),
          SLOT(updateButtonAccess(int)));
}

void ProviderActionNavigator::updateButtonAccess(int index) {
  if (index > 0) {
    btn_back->setEnabled(true);
    btn_next->setEnabled((index + 1) < stackedWidget->count());
  } else {
    btn_back->setEnabled(false);
    btn_next->setEnabled(true);
  }
}

void ProviderActionNavigator::prepareGotoSignal() {
  QPushButton *btn = qobject_cast<QPushButton *>(sender());
  if (btn == nullptr)
    return;

  int _i = stackedWidget->currentIndex();
  int _c = stackedWidget->count();
  int _s = -1; // send index
  if (btn->objectName().startsWith("forward") && _i < _c) {
    _s = (_i + 1);
  } else if (_i > 0) {
    _s = (_i - 1);
  }

  if (_s < 0)
    return;

  emit sendGotoPage(_s);
}

} // namespace AntiquaCRM
