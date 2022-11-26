// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ASSISTANT_H
#define ASSISTANT_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QStackedWidget>
#include <QWidget>
#include <QEvent>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QSettings>
#include <QStatusBar>
#include <QPushButton>

class InfoPage;
class ConfigPgSQL;
class ConfigSSL;
class TestConfig;

class Assistant : public QDialog {
  Q_OBJECT

private:
  bool unsaved = true;
  QSettings *cfg;
  QStackedWidget *m_stackedWidget;
  InfoPage *m_page0;
  InfoPage *m_page1;
  ConfigPgSQL *m_configPgSQL;
  ConfigSSL *m_configSSL;
  TestConfig *m_lastTest;
  QDialogButtonBox *m_buttonBox;
  QStatusBar *m_statusBar;
  QPushButton *btn_close;
  QString p_lastError;
  int index();
  void goTo(int index = 0);
  void setButtonBox();
  bool connectionTest();
  void keyPressEvent(QKeyEvent *);
  void closeEvent(QCloseEvent *);
  bool event(QEvent *);
  void migrationData(const QString &section);

private Q_SLOTS:
  void restart();
  void previousPage();
  void nextPage();
  void beforeAccept();
  void beforeClose();
  void runTest();
  void save();
  void pageEntered(int);

public:
  explicit Assistant(QWidget *parent = nullptr);
  ~Assistant();
};
#endif // ASSISTANT_H
