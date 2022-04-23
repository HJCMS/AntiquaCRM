// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "messagebox.h"

#include <QtCore/QDebug>

MessageBox::MessageBox(QWidget *parent) : QMessageBox{parent} {
  setObjectName("MessageBox");
  setDefaultButton(QMessageBox::Ok);
  setTextFormat(Qt::PlainText);
  setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard);
}

void MessageBox::buildTimerMessage() {
  QString data = message;
  data.append("\n");
  data.append(tr("Message closed in "));
  data.append(QString::number(timeout));
  data.append(" ");
  data.append(tr("Seconds"));
  setText(data);
}

void MessageBox::timerEvent(QTimerEvent *ev) {
  timeout = (timeout - 1);
  if (timeout < 1) {
    killTimer(timerID);
    accept();
  }
  buildTimerMessage();
}

void MessageBox::notice(const QString &msg) {
  setIcon(QMessageBox::Warning);
  setText(msg);
  exec();
}

void MessageBox::failed(const QString &err, const QString &details) {
  setIcon(QMessageBox::Critical);
  setText(tr("SQL Syntaxerror"));
  setInformativeText(err);
  if (!details.isEmpty())
    setDetailedText(details);

  exec();
}

void MessageBox::success(const QString &msg, int s) {
  timeout = s;
  message = msg;
  setIcon(QMessageBox::Information);
  setText(msg);
  timerID = startTimer(1000);
  buildTimerMessage();
  exec();
}
