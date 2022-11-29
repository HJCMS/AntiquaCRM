// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "messageeditor.h"

MessageEditor::MessageEditor(QWidget *parent) : QTextEdit{parent} {
  setObjectName("messages_editor");
  setAcceptDrops(true);
  setAcceptRichText(false);
  setContentsMargins(0, 0, 0, 0);
  setAutoFormatting(QTextEdit::AutoNone);
  setTextInteractionFlags(Qt::TextEditorInteraction);

  QPalette p = palette();
  p.setColor(QPalette::Base, Qt::white);
  p.setColor(QPalette::Text, Qt::black);
  setPalette(p);

  QString css("QTextEdit { color: black; }");
  document()->setDefaultStyleSheet(css);
  document()->setModified(true);
}

void MessageEditor::setBody(const QString &txt) {
  clear();
  setPlainText(txt);
}
