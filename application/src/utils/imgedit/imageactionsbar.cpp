// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imageactionsbar.h"

ImageActionsBar::ImageActionsBar(QWidget *parent) : QToolBar{parent} {
  setContentsMargins(10, 0, 0, 0);
  setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

  ac_reset = addAction(tr("Reset"));
  ac_reset->setIcon(QIcon("://icons/action_redo.png"));
  connect(ac_reset, SIGNAL(triggered()), SIGNAL(sendReset()));

  ac_cut = addAction(tr("Cutting"));
  ac_cut->setIcon(QIcon("://icons/action_cut.png"));
  ac_cut->setEnabled(false);
  connect(ac_cut, SIGNAL(triggered()), SIGNAL(sendCutting()));

  ac_scale = addAction(tr("Scale"));
  ac_scale->setIcon(QIcon("://icons/view_scale.png"));
  ac_scale->setEnabled(false);
  connect(ac_scale, SIGNAL(triggered()), SIGNAL(sendAdjust()));

  ac_rotate = addAction(tr("Rotate"));
  ac_rotate->setIcon(QIcon("://icons/view_rotate_right.png"));
  ac_rotate->setEnabled(false);
  connect(ac_rotate, SIGNAL(triggered()), SIGNAL(sendRotate()));
  addSeparator();

  ac_save = addAction(tr("Save"));
  ac_save->setIcon(QIcon("://icons/action_save.png"));
  connect(ac_save, SIGNAL(triggered()), SIGNAL(sendSave()));
  addSeparator();

  ac_close = addAction(tr("Finish"));
  ac_close->setIcon(QIcon("://icons/action_quit.png"));
  connect(ac_close, SIGNAL(triggered()), SIGNAL(sendAccept()));
}

void ImageActionsBar::enableCustomActions(bool b) {
  ac_cut->setEnabled(b);
  ac_scale->setEnabled(b);
  ac_rotate->setEnabled(b);
}
