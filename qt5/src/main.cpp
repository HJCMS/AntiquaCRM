// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#include "antiquaappl.h"

#include <QApplication>
#include <QProcess>
#include <QTextCodec>
#include <QtGlobal>

#ifdef Q_OS_LINUX
#include <cstdio>
#include <syslog.h>

const char *functionInfo(const QMessageLogContext &context) {
  QString b = QString::asprintf("’%s’", context.function);
  return b.toLocal8Bit().constData();
}

void SyslogMessageHandler(QtMsgType type, // Message Type
                          const QMessageLogContext &context,
                          const QString &msg) {
  QByteArray localMsg = msg.toLocal8Bit();
  switch (type) {
  case QtInfoMsg:
    syslog(LOG_INFO, "%s", localMsg.constData());
    break;

  case QtWarningMsg:
    syslog(LOG_WARNING, "%s", localMsg.constData());
    break;

  case QtCriticalMsg:
  case QtFatalMsg:
    syslog(LOG_ERR, "%s %s", localMsg.constData(), functionInfo(context));
    abort();

  default:
    syslog(LOG_DEBUG, "%s", localMsg.constData());
    break;
  }
}
#endif

int main(int argc, char *argv[]) {
#ifndef ANTIQUA_DEVELOPEMENT
#ifdef Q_OS_LINUX
  qInstallMessageHandler(SyslogMessageHandler);
#endif
#endif

  AntiquaAppl *antiqua = new AntiquaAppl(argc, argv);
  antiqua->setQuitOnLastWindowClosed(false);
  antiqua->initDefaultTheme();

  if (QTextCodec::codecForLocale()->name() != "UTF-8") {
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  }

  if (argc > 0) {
    for (int i = 0; i <= argc; i++) {
      QString arg = QString(argv[i]).toLower();
      arg.replace("-", "");
      arg.replace(" ", "");
      if (arg.trimmed() == "assistant") {
        QProcess::execute("antiquacrm_assistant", QStringList());
        return 0;
      }
    }
  }

  if (antiqua->isRunning()) {
    qWarning("AntiquaCRM is already up!");
    return 0;
  }

  Q_INIT_RESOURCE(resources);

  return antiqua->exec();
}
