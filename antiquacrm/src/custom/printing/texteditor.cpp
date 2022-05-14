// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "texteditor.h"

#include <QDebug>
#include <QPalette>

TextEditor::TextEditor(QWidget *parent) : QTextEdit{parent}
{
  setObjectName("text_editor");
  QPalette p = palette();
  p.setColor(QPalette::Base,Qt::white);
  p.setColor(QPalette::Text,Qt::black);
  setPalette(p);
  setWindowModified(true);
}
