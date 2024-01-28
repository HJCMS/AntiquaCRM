// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "templateeditor.h"

#include <QMimeData>

TemplateEditor::TemplateEditor(QWidget *parent) : QTextEdit{parent} {
  setObjectName("tpl_editor");
  setContentsMargins(0, 0, 0, 0);
  setAutoFormatting(QTextEdit::AutoNone);
  setTextInteractionFlags(Qt::TextEditorInteraction);
  setAcceptDrops(true);
  setAcceptRichText(false);

  QPalette p = palette();
  p.setColor(QPalette::Base, Qt::white);
  p.setColor(QPalette::Text, Qt::black);
  setPalette(p);

  document()->setDefaultStyleSheet("QTextEdit {color:black;}");
  document()->setModified(true);
}

void TemplateEditor::dragEnterEvent(QDragEnterEvent *event) {
  const QMimeData *m_md = event->mimeData();
  if (m_md->hasText()) {
    const QString _m = m_md->text();
    bool _b = (!_m.isEmpty() && _m.startsWith("@") && _m.endsWith("@"));
    event->setAccepted(_b);
    if (_b) {
      event->setDropAction(Qt::MoveAction);
      event->accept();
      return;
    }
  }
  event->ignore();
}

void TemplateEditor::dragMoveEvent(QDragMoveEvent *event) {
  const QMimeData *m_md = event->mimeData();
  if (m_md->hasText()) {
    if (event->dropAction() & ~Qt::MoveAction)
      event->setDropAction(Qt::MoveAction);

    event->setAccepted(true);
    event->accept();
    return;
  }
  event->ignore();
}

void TemplateEditor::setBody(const QString &data) {
  clear();
  setPlainText(data);
}

const QString TemplateEditor::getBody() {
  QString _data = toPlainText();
  return _data.trimmed();
}
