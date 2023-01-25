// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CDTRACKING_CDREADDIALOG_H
#define CDTRACKING_CDREADDIALOG_H

#include <AntiquaCRM>
#include <AntiquaWidgets>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFrame>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStatusBar>
#include <QTextEdit>
#include <QUrl>

class TracksListWidget;

class CDReadDialog final : public QDialog {
  Q_OBJECT

private:
  AntiquaCRM::ASettings *m_cfg;
  QFrame *m_mainFrame;
  AntiquaILabel *infolabel;
  LineEdit *m_title;
  LineEdit *m_artists;
  YearEdit *m_year;
  LineEdit *m_barcode;
  TracksListWidget *m_tracksList;
  QDialogButtonBox *m_btnBox;
  QStatusBar *m_statusBar;

  const QJsonObject getRelease(const QJsonArray &);
  void getCDInfo();

private Q_SLOTS:
  void queryResponses(const QJsonDocument &);
  void setQueryDiscId(const QUrl &);

public:
  explicit CDReadDialog(QWidget *parent = nullptr);
  int exec() override;
  const QJsonObject data();
  ~CDReadDialog();
};

#endif // CDTRACKING_CDREADDIALOG_H
