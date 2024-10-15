// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#include "application.h"
#include <AntiquaCRM>

#ifdef Q_OS_LINUX
#  include <syslog.h>

void SyslogMessageHandler(QtMsgType type, // Message Type
                          const QMessageLogContext& context, const QString& msg) {
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
      {
        QString _context = QString::asprintf("’%s’", context.function);
        syslog(LOG_ERR, "%s %s", localMsg.constData(), _context.toLocal8Bit().constData());

        abort();
      }
      break;

    default:
      break;
  }
}
#endif

int main(int argc, char* argv[]) {
#ifdef Q_OS_LINUX
#  ifndef ANTIQUA_DEVELOPMENT
  qInstallMessageHandler(SyslogMessageHandler);
#  endif
#endif

  // We use a custom session manager
  QApplication::setAttribute(Qt::AA_DisableSessionManager, true);

  // Ensure that color palettes and font propagation are not inherited.
  QApplication::setAttribute(Qt::AA_UseStyleSheetPropagationInWidgetStyles, false);

#ifdef Q_OS_WIN
  /*
   * Is the attempt to display the selected font a little better if the font
   * scaling is faulty.
   *
   * Detailed information at https://doc.qt.io/qt-5/highdpi.html
   *
   * @warning Not do this on XCB Desktop systems!
   */
  qputenv("QT_ENABLE_HIGHDPI_SCALING", "1");
  if (!qEnvironmentVariableIsSet("QT_SCALE_FACTOR")) {
    qputenv("QT_SCALE_FACTOR", "1.075");
    QApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
  }
#endif

  Application* m_app = new Application(argc, argv);
  if (m_app->isRunning()) {
    qWarning("AntiquaCRM is already up!");
    return 0;
  }

  return m_app->exec();
}
