// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "texteditor.h"

#include <QDebug>
#include <QPalette>

TextEditor::TextEditor(QWidget *parent) : QTextEdit{parent}
{
  setContentsMargins(0, 0, 0, 0);
  QPalette p = palette();
  p.setColor(QPalette::Base,Qt::white);
  p.setColor(QPalette::Text,Qt::black);
  setPalette(p);
  setWindowModified(true);

  QString css("* { color: black; }");
  css.append("p, li { white-space: pre-wrap; }");
  document()->setDefaultStyleSheet(css);
  document()->setModified(true);
}
