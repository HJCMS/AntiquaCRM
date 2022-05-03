// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_MAPPLICATION_H
#define ANTIQUACRM_MAPPLICATION_H

#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <QtNetwork/QLocalServer>
#include <QtWidgets/QApplication>

class ApplSettings;
class MWindow;
class SocketServer;

namespace HJCMS {
  class SqlCore;
};

class MApplication : public QApplication {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  HJCMS::SqlCore *m_sqlDB;
  ApplSettings *m_settings;
  MWindow *m_mainWindow;
  SocketServer *m_socket;
  bool initialSocketServer();

public:
  explicit MApplication(int &argc, char **argv);
  bool isRunning();
  int exec();
  virtual ~MApplication();
};

#endif // ANTIQUACRM_H
