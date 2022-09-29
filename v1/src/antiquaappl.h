// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_APPLICATION_H
#define ANTIQUACRM_APPLICATION_H

#include <QApplication>
#include <QObject>
#include <QScreen>
#include <QStyle>
#include <AntiquaCRM>

class AntiquaWindow;
class AntiquaSplashScreen;
class AntiquaSystemTray;

class AntiquaAppl : public QApplication {
  Q_OBJECT

private:
  AntiquaCRM::ASettings *m_cfg;
  AntiquaWindow *m_mainWindow;
  AntiquaSplashScreen *m_splash;
  AntiquaSystemTray *m_systemTray;
  AntiquaCRM::ASqlCore *m_sql;

public:
  explicit AntiquaAppl(int &argc, char **argv);
  void initDefaultTheme();
  bool isRunning();
  int exec();
  ~AntiquaAppl();
};

#endif // ANTIQUACRM_APPLICATION_H
