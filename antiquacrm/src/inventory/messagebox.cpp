// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "messagebox.h"

#include <QtCore/QDebug>

MessageBox::MessageBox(QWidget *parent) : QMessageBox{parent} {
  setObjectName("MessageBox");
  setDefaultButton(QMessageBox::Ok);
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

void MessageBox::noticeMessage(const QString &msg) {
  setIcon(QMessageBox::Warning);
  setText(msg);
  exec();
}

void MessageBox::queryFail(const QString &msg) {
  setIcon(QMessageBox::Critical);
  setText(msg);
  exec();
}

void MessageBox::querySuccess(const QString &msg, int s) {
  timeout = s;
  message = msg;
  setIcon(QMessageBox::Information);
  setText(msg);
  timerID = startTimer(1000);
  buildTimerMessage();
  exec();
}
