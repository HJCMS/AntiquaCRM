// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CDTRACKING_CDDISCID_H
#define CDTRACKING_CDDISCID_H

#include <QMetaType>
#include <QObject>
#include <QString>
#include <QThread>
#include <QUrl>

/**
 * @brief The CDDiscId class
 * @ingroup utils
 */
class CDDiscId final : public QThread {
  Q_OBJECT

private:
  QString p_brainz_id;
  QString p_brainz_toc;
  QString p_cddb_id;
  QString p_cddb_toc;
  QString p_gtin;
  bool isBarcocde(const QString &);
  bool open();

Q_SIGNALS:
  void sendStatusMessage(const QString &);
  void sendBarcode(const QString &);
  void sendQuery(const QUrl &);

public:
  explicit CDDiscId(QObject *parent = nullptr);
  const QUrl queryMusicBrainz();
  const QUrl queryCDDB();
  void run() override;
};

#endif // CDTRACKING_CDDISCID_H
