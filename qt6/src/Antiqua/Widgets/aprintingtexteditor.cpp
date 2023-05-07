// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aprintingtexteditor.h"

#include <QPalette>

namespace AntiquaCRM {

APrintingTextEditor::APrintingTextEditor(QWidget *parent) : QTextEdit{parent} {
  QPalette p = palette();
  p.setColor(QPalette::Base, Qt::white);
  p.setColor(QPalette::Text, Qt::black);
  setPalette(p);

  setTextColor(Qt::black);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  QString css("* { color: black; }");
  css.append("p, li { white-space: pre-wrap; }");
  document()->setDefaultStyleSheet(css);
  document()->setModified(true);
}

} // namespace AntiquaCRM
