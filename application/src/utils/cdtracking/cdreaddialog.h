// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CDTRACKING_CDREADDIALOG_H
#define CDTRACKING_CDREADDIALOG_H

#include <AntiquaCRM>
#include <QDialog>
#include <QTextEdit>
#include <QDialogButtonBox>
#include <QStatusBar>
#include <QToolBar>
#include <QWidget>

#include "cdinfo.h"

class CDReadDialog final : public QDialog {
  Q_OBJECT

private:
  AntiquaCRM::ASettings *m_cfg;
  QToolBar *m_toolBar;
  QTextEdit *m_hwInfo;
  QWidget *m_centralWidget;
  QDialogButtonBox *m_btnBox;
  QStatusBar *m_statusBar;

//void keyPressEvent(QKeyEvent *) override;
//bool event(QEvent *) override;

  void getCDInfo();

private Q_SLOTS:
  void getMediaInfo(const HardwareInfo &);

public:
  explicit CDReadDialog(QWidget *parent = nullptr);
  int exec() override;
  ~CDReadDialog();
};

#endif // CDTRACKING_CDREADDIALOG_H
