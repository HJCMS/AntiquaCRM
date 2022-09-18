// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_APPLICATION_H
#define ANTIQUA_APPLICATION_H

#include <QApplication>
#include <QObject>
#include <QScreen>
#include <QStyle>
#include <AntiquaCRM>

class AntiquaWindow;
class SystemTray;

class AntiquaAppl : public QApplication {
  Q_OBJECT

private:
  AntiquaCRM::ASqlCore *m_sql;
  AntiquaWindow *m_mainWindow;
  SystemTray *m_systemTray;

private Q_SLOTS:
  // void showMainWindow();

public:
  explicit AntiquaAppl(int &argc, char **argv);
  void initDefaultTheme();
  bool isRunning();
  int exec();
  ~AntiquaAppl();
};

#endif // ANTIQUA_APPLICATION_H
