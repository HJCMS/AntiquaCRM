// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CDTRACKING_CDREADDIALOG_H
#define CDTRACKING_CDREADDIALOG_H

#include <AntiquaCRM>
#include <AntiquaWidgets>
#include <QDialog>
#include <QDomDocument>
#include <QDialogButtonBox>
#include <QFrame>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStatusBar>
#include <QTextEdit>
#include <QUrl>

class TracksListWidget;
class SelectGenre;

/**
 * @defgroup AntiquaCDTracking CD Tracking
 * @brief Read CD’s Metadata Dialog with discid
 */
class CDReadDialog final : public QDialog {
  Q_OBJECT

private:
  QFrame *m_mainFrame;
  AntiquaILabel *infolabel;
  LineEdit *m_title;
  LineEdit *m_artists;
  SelectGenre *m_genre;
  YearEdit *m_year;
  LineEdit *m_barcode;
  TracksListWidget *m_tracksList;
  QDialogButtonBox *m_btnBox;
  QStatusBar *m_statusBar;

  const QJsonObject getRelease(const QJsonArray &);
  void getCDInfo();

private Q_SLOTS:
  void setBarcode(const QString &);
  void queryResponses(const QJsonDocument &);
  void createMetaQuery(const QUrl &);

public:
  explicit CDReadDialog(QWidget *parent = nullptr);
  int exec() override;
  const QJsonObject data();
  ~CDReadDialog();
};

#endif // CDTRACKING_CDREADDIALOG_H
