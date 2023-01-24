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

class CDDiscId final : public QThread {
  Q_OBJECT

private:
  QString p_brainz_id;
  QString p_brainz_toc;
  QUrl p_submissionUrl;
  bool open();

Q_SIGNALS:
  void sendStatusMessage(const QString &);
  void sendQueryDiscChanged(const QUrl &);

public:
  explicit CDDiscId(QObject *parent = nullptr);
  void run() override;
};

#endif // CDTRACKING_CDDISCID_H
