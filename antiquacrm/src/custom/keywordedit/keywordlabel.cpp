// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "keywordlabel.h"

#include <QDebug>
#include <QPainter>
#include <QPalette>
#include <QSizePolicy>
#include <QStyleOptionFrame>

KeywordLabel::KeywordLabel(const QString &txt, QWidget *parent)
    : QLabel{txt, parent} {
  setObjectName(txt);
  setContentsMargins(0, 0, 0, 0);
  setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
  setFrameShape(QFrame::Box);
  setFrameShadow(QFrame::Raised);
  // setFrameStyle(QFrame::Shape_Mask);
  setLineWidth(1);
}

/*
void KeywordLabel::paintEvent(QPaintEvent *e) {
  QRect r = frameRect();

  // QStyleOption::SO_Frame
  QStyleOptionFrame option;
  option.initFrom(this);

  QPainter painter(this);
  painter.end();
  QLabel::paintEvent(e);
}
*/
