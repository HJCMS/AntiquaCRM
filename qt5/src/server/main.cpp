// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#include "antiquacmd.h"

#include <QByteArray>
#include <QString>
#include <QtGlobal>
#include <cstdio>
#include <syslog.h>
#include <QTextCodec>

void SyslogMessageHandler(QtMsgType type, const QMessageLogContext &context,
                          const QString &msg) {
  Q_UNUSED(context)
  QByteArray localMsg = msg.toLocal8Bit();
  switch (type) {
  case QtInfoMsg:
    syslog(LOG_INFO, "AntiquaCRM (Info): %s", localMsg.constData());
    break;

  case QtWarningMsg:
    syslog(LOG_WARNING, "AntiquaCRM (Warning): %s", localMsg.constData());
    break;

  case QtCriticalMsg:
  case QtFatalMsg:
    syslog(LOG_ALERT, "AntiquaCRM (Error): %s", localMsg.constData());
    abort();

  default:
    printf("AntiquaCRM (Debug): %s", localMsg.constData());
    break;
  }
}

int main(int argc, char *argv[]) {
#ifndef ANTIQUA_DEVELOPEMENT
  qInstallMessageHandler(SyslogMessageHandler);
#endif
  AntiquaCMD *m_appl = new AntiquaCMD(argc, argv);

  if (QTextCodec::codecForLocale()->name() != "UTF-8") {
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  }

  return m_appl->exec();
}
