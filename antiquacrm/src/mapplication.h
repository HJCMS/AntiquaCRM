// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_MAPPLICATION_H
#define ANTIQUACRM_MAPPLICATION_H

#include <QEvent>
#include <QObject>
#include <QLocalServer>
#include <QStyle>
#include <QApplication>

#include <AntiquaCRM>

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
  HJCMS::SqlCore *m_sql;
  ApplSettings *m_config;
  MWindow *m_window;
  SocketServer *m_socket;
  bool initialSocketServer();
  bool initTranslations();
  void openAssistant();

public:
  explicit MApplication(int &argc, char **argv);
  bool isRunning();
  void initThemeStyle();
  int exec();
  virtual ~MApplication();
};

#endif // ANTIQUACRM_H
