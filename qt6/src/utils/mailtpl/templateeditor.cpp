// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "templateeditor.h"

TemplateEditor::TemplateEditor(QWidget *parent) : QTextEdit{parent} {
  setAcceptDrops(true);
  setAcceptRichText(false);
  setContentsMargins(0, 0, 0, 0);
  setAutoFormatting(QTextEdit::AutoNone);
  setTextInteractionFlags(Qt::TextEditorInteraction);

  QPalette p = palette();
  p.setColor(QPalette::Base, Qt::white);
  p.setColor(QPalette::Text, Qt::black);
  setPalette(p);

  document()->setDefaultStyleSheet("QTextEdit {color:black;}");
  document()->setModified(true);
}

void TemplateEditor::setBody(const QString &data) {
  clear();
  setPlainText(data);
}

const QString TemplateEditor::getBody() {
  QString _data = toPlainText();
  return _data.trimmed();
}
