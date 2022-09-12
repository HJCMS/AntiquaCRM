// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_APPLICATION_H
#define ANTIQUACRM_APPLICATION_H

#include <QApplication>
#include <QObject>
#include <QScreen>
#include <QStyle>

class AntiquaWindow;
class SystemTray;

class AntiquaCRM : public QApplication {
  Q_OBJECT

private:
  AntiquaWindow *m_mainWindow;
  SystemTray *m_systemTray;

private Q_SLOTS:
  // void showMainWindow();

public:
  explicit AntiquaCRM(int &argc, char **argv);
  void initDefaultTheme();
  bool isRunning();
  int exec();
  ~AntiquaCRM();
};

#endif // ANTIQUACRM_APPLICATION_H
