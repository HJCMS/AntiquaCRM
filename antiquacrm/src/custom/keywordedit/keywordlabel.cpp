// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "keywordlabel.h"

#include <QPalette>
#include <QSizePolicy>

KeywordLabel::KeywordLabel(const QString &txt, QWidget *parent)
    : QLabel{txt, parent} {
  setObjectName(txt);
  setContentsMargins(0, 0, 0, 0);
  setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
  setFrameShape(QFrame::StyledPanel);
  setFrameShadow(QFrame::Plain);
  setStyleSheet(lableStyleSheet());
}

const QString KeywordLabel::lableStyleSheet() const {
  QString color = palette().color(QPalette::Text).name();
  QStringList css;
  css << "padding-left:5px;padding-right:5px;";
  css << "padding-top:1px;padding-bottom:1px;";
  css << "border:1px solid " + color + ";border-radius:10px;";
  return "KeywordLabel {" + css.join("") + "}";
}
