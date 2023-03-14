// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "mailbody.h"

#include <QDebug>
#include <QPalette>
#include <QTextDocument>
#include <QTextCursor>

MailBody::MailBody(QWidget *parent) : QTextEdit{parent} {
  setObjectName("mail_body");
  setAcceptDrops(true);
  setContentsMargins(0, 0, 0, 0);

  QPalette p = palette();
  p.setColor(QPalette::Base, Qt::white);
  p.setColor(QPalette::Text, Qt::black);
  setPalette(p);

  QString css("QTextEdit { color: black; }");
  document()->setDefaultStyleSheet(css);
  document()->setModified(true);
}

void MailBody::setFocus() {
  QTextEdit::setFocus();
  QTextCursor cursor = cursorForPosition(QPoint(0,0));
  setTextCursor(cursor);
}
