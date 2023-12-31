// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#include <AntiquaCRM>
#include <QByteArray>
#include <QString>
#include <QtGlobal>
#include <cstdio>
#include <syslog.h>

#include "antiquacmd.h"

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
  case QtFatalMsg: {
    QString _context = QString::asprintf("’%s’", context.function);
    syslog(LOG_ERR, "%s %s", localMsg.constData(),
           _context.toLocal8Bit().constData());

    abort();
  } break;

  default:
    break;
  }
}

int main(int argc, char *argv[]) {
#ifndef ANTIQUA_DEVELOPEMENT
  qInstallMessageHandler(SyslogMessageHandler);
#endif
  AntiquaCMD *m_appl = new AntiquaCMD(argc, argv);
  return m_appl->exec();
}
