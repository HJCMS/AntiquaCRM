// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_APPLICATION_H
#define ANTIQUACRM_APPLICATION_H

#include <AntiquaCRM>
#include <QApplication>
#include <QJsonObject>
#include <QList>
#include <QObject>
#include <QScreen>
#include <QStyle>

class AntiquaWindow;
class AntiquaSystemTray;
class AntiquaTimer;

class AntiquaAppl : public QApplication {
  Q_OBJECT

private:
  AntiquaCRM::ASettings *m_cfg;
  AntiquaWindow *m_mainWindow;
  AntiquaSystemTray *m_systemTray;
  AntiquaCRM::ASqlCore *m_sql;
  AntiquaTimer *m_timer;

  QList<AntiquaCRM::APluginInterface *> p_interfaces;

  void initGui();

  bool checkInterfaces();
  bool checkRemotePort();
  bool checkDatabase();
  bool initTranslations();

Q_SIGNALS:
  void sendStatusMessage(const QString &msg);

public Q_SLOTS:
  void applicationQuit();

public:
  explicit AntiquaAppl(int &argc, char **argv);
  void startAssistant();
  void initDefaultTheme();
  bool isRunning();
  int exec();
};

#endif // ANTIQUACRM_APPLICATION_H
