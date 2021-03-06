// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "messagebox.h"

MessageBox::MessageBox(QWidget *parent) : QMessageBox{parent} {
  setObjectName("antiqua_message_box");
  setWindowFlag(Qt::Dialog, true);
#ifdef Q_WS_WIN
  setWindowFlag(Qt::MSWindowsFixedSizeDialogHint, false);
#endif
  setDefaultButton(QMessageBox::Ok);
  // Warning !!! Kein RichText wegen tr().arg() verwenden!!!
  setTextFormat(Qt::PlainText);
  setTextInteractionFlags(Qt::TextSelectableByMouse);
  setSizeGripEnabled(true);
  setMinimumSize(400, 200);
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

int MessageBox::notice(const QString &msg) {
  setIcon(QMessageBox::Warning);
  setText(msg);
  return exec();
}

int MessageBox::failed(const QString &err, const QString &details) {
  setIcon(QMessageBox::Critical);
  setText(tr("The last Operation answered with an Error!"));
  if (!details.isEmpty())
    setDetailedText(details);

  setInformativeText(err);
  return exec();
}

int MessageBox::success(const QString &msg, int s) {
  timeout = s;
  message = msg;
  setIcon(QMessageBox::Information);
  setText(msg);
  timerID = startTimer(1000);
  buildTimerMessage();
  return exec();
}
