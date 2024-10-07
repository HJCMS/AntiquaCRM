// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUA_ASSISTANT_H
#define ANTIQUA_ASSISTANT_H

#include <AntiquaCRM>
#include <AntiquaWidgets>
#include <QCloseEvent>
#include <QDialog>
#include <QDialogButtonBox>
#include <QEvent>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QStackedWidget>
#include <QStatusBar>
#include <QWidget>

class ConfigGeneral;
class ConfigDatabase;

class Assistant final : public QDialog {
  Q_OBJECT

private:
  bool unsaved = true;
  QStackedWidget* m_stackedWidget;
  ConfigDatabase* m_page1;
  ConfigGeneral* m_page2;
  QDialogButtonBox* m_buttonBox;
  QStatusBar* m_statusBar;
  QPushButton* btn_close;
  QString p_lastError;
  void setButtonBox();
  int getPage();
  void keyPressEvent(QKeyEvent*);
  void closeEvent(QCloseEvent*);
  bool event(QEvent*);

private Q_SLOTS:
  void restart();
  void setPage(int index = 0);
  void previousPage();
  void nextPage();
  void beforeAccept();
  void beforeClose();
  void pageChanged(int);
  void save();

public:
  explicit Assistant(QWidget* parent = nullptr);
  ~Assistant();
};

#endif // ANTIQUA_ASSISTANT_H
