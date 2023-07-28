// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "atextedit.h"

#include <QRegularExpression>
#include <QSizePolicy>

namespace AntiquaCRM {

ATextEdit::ATextEdit(QWidget *parent) : QTextEdit{parent} {
  setObjectName("ATextEdit");
  setTextInteractionFlags(Qt::TextEditorInteraction);
  setAutoFormatting(QTextEdit::AutoNone);
  setFocusPolicy(Qt::ClickFocus);
  setAcceptDrops(false);
  setAcceptRichText(false);
  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
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

void ATextEdit::setText(const QString &txt) {
  setPlainText(strip(txt));
}

const QString ATextEdit::text() {
  return strip(toPlainText());
}

const QString ATextEdit::strip(const QString &str) {
  QString _buf = str.trimmed();
  _buf.replace("\"", "’");
  _buf.replace("'", "’");
  _buf.replace("<", "«");
  _buf.replace(">", "»");
  static const QRegularExpression tabs("\\t+",
                                       QRegularExpression::NoPatternOption);
  _buf.replace(tabs, " ");
  _buf.squeeze();
  return _buf.trimmed();
}

} // namespace AntiquaCRM
