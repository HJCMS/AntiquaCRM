// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooksactionsbar.h"

#include <AntiquaWidgets>
#include <QSizePolicy>

AbeBooksActionsBar::AbeBooksActionsBar(QWidget *parent) : QToolBar{parent} {
  setOrientation(Qt::Horizontal);
  setToolButtonStyle(Qt::ToolButtonIconOnly);

  ac_left = addAction(AntiquaCRM::antiquaIcon("arrow-left"), QString());
  ac_left->setObjectName("go_back");
  ac_left->setToolTip(tr("Previous"));

  ac_right = addAction(AntiquaCRM::antiquaIcon("arrow-right"), QString());
  ac_right->setObjectName("go_next");
  ac_right->setToolTip(tr("Next"));

  QWidget *spacer = new QWidget(this);
  spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  insertWidget(ac_right, spacer);

  connect(ac_left, SIGNAL(triggered()), SLOT(goToPage()));
  connect(ac_right, SIGNAL(triggered()), SLOT(goToPage()));
}

void AbeBooksActionsBar::goToPage() {
  QAction *ac = qobject_cast<QAction *>(sender());
  if (ac == nullptr)
    return;

  if (ac->objectName().startsWith("go_back"))
    emit goPrevious();

  if (ac->objectName().startsWith("go_next"))
    emit goNext();
}
