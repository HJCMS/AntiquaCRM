// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "atextedit.h"

namespace AntiquaCRM {

ATextEdit::ATextEdit(QWidget *parent) : QTextEdit{parent} {
  setTextInteractionFlags(Qt::TextEditorInteraction);
  setAutoFormatting(QTextEdit::AutoNone);
  setFocusPolicy(Qt::ClickFocus);
  setAcceptDrops(false);
  setAcceptRichText(false);
  ungrabGesture(Qt::TapGesture);
}

void ATextEdit::keyPressEvent(QKeyEvent *event) {
  if (event->type() == QEvent::KeyPress) {
    if (event->key() == Qt::Key_Tab) {
      event->setAccepted(false);
      return;
    }
  }
  QTextEdit::keyPressEvent(event);
}

void ATextEdit::keyReleaseEvent(QKeyEvent *event) {
  if (event->type() == QEvent::KeyRelease) {
    if (event->key() == Qt::Key_Tab) {
      event->setAccepted(false);
      return;
    }
  }
  QTextEdit::keyReleaseEvent(event);
}

const QRegularExpression ATextEdit::pattern() {
  return QRegularExpression("(\\s+|\\t+)");
}

void ATextEdit::setText(const QString &txt) {
  QString _text = strip(txt);
  setPlainText(_text);
}

const QString ATextEdit::strip(const QString &str) const {
  QString _buf = str.trimmed();
  _buf.replace("'", "’");
  _buf.replace("<", "«");
  _buf.replace(">", "»");
  _buf.replace(pattern(), " ");
  return _buf.trimmed();
}

const QString ATextEdit::text() {
  QString _text = toPlainText();
  return strip(_text);
}

} // namespace AntiquaCRM
