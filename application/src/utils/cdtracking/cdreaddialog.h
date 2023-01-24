// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CDTRACKING_CDREADDIALOG_H
#define CDTRACKING_CDREADDIALOG_H

#include <AntiquaCRM>
#include <QDialog>
#include <QDialogButtonBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStatusBar>
#include <QTextEdit>
#include <QToolBar>
#include <QUrl>
#include <QWidget>

class CDReadDialog final : public QDialog {
  Q_OBJECT

private:
  AntiquaCRM::ASettings *m_cfg;
  QToolBar *m_toolBar;
  QTextEdit *m_hwInfo;
  QWidget *m_centralWidget;
  QDialogButtonBox *m_btnBox;
  QStatusBar *m_statusBar;

  const QJsonObject getRelease(const QJsonArray &);
  void getCDInfo();

private Q_SLOTS:
  void queryResponses();
  // void queryResponses(const QJsonDocument &);
  void setQueryDiscId(const QUrl &);

public:
  explicit CDReadDialog(QWidget *parent = nullptr);
  int exec() override;
  ~CDReadDialog();
};

#endif // CDTRACKING_CDREADDIALOG_H
