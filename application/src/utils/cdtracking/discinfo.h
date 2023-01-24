// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CDTRACKING_DISCINFO_H
#define CDTRACKING_DISCINFO_H

#include <QDate>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>

class DiscInfo final : public QJsonObject {
public:
  struct Track {
    int index;
    QString title;
    int length;
  };
  explicit DiscInfo(const QJsonObject &parent);
  const QString getTitle();
  const QString getArtists();
  const QString getBarcode();
  const QList<Track> getTracks();
  int getReleaseYear();
};
Q_DECLARE_METATYPE(DiscInfo::Track);

#endif // CDTRACKING_DISCINFO_H
