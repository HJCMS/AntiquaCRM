// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#include <AntiquaCRM>
#include <QByteArray>
#include <QString>
#include <QtGlobal>
#include <syslog.h>

#include "antiquacmd.h"

void SyslogMessageHandler(QtMsgType type, // Message Type
                          const QMessageLogContext &context,
                          const QString &msg) {
  QByteArray _data = msg.toLocal8Bit();
  switch (type) {
  case QtInfoMsg:
    syslog(LOG_INFO, "%s", _data.constData());
    break;

  case QtDebugMsg:
  case QtWarningMsg:
    syslog(LOG_WARNING, "%s", _data.constData());
    break;

  case QtCriticalMsg:
  case QtFatalMsg: {
    QString _context = QString::asprintf("’%s’", context.function);
    syslog(LOG_ERR, "%s %s", _data.constData(),
           _context.toLocal8Bit().constData());

    abort();
  } break;

  default:
    break;
  }
  _data.clear();
}

int main(int argc, char *argv[]) {
#ifndef ANTIQUA_DEVELOPMENT
  qInstallMessageHandler(SyslogMessageHandler);
#endif
  // qputenv("QT_DEBUG_PLUGINS", QByteArray("1"));
  AntiquaCMD *m_appl = new AntiquaCMD(argc, argv);
  return m_appl->exec();
}
