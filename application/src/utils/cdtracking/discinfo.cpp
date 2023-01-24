// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "discinfo.h"

DiscInfo::DiscInfo(const QJsonObject &parent) : QJsonObject{parent} {}

const QString DiscInfo::getTitle() { return value("title").toString(); }

const QString DiscInfo::getArtists() {
  QStringList artists;
  QJsonArray array = value("artist-credit").toArray();
  for (int i = 0; i < array.size(); i++) {
    QJsonObject obj = array[i].toObject();
    artists << obj.value("name").toString().trimmed();
  }
  return artists.join(",");
}

const QString DiscInfo::getBarcode() { return value("barcode").toString(); }

const QList<DiscInfo::Track> DiscInfo::getTracks() {
  QList<DiscInfo::Track> list;
  QJsonArray array = value("media").toArray();
  QJsonArray tracks;
  QJsonObject first = array.first().toObject();
  QJsonObject::iterator it;
  for (it = first.begin(); it != first.end(); ++it) {
    if (it.key() == "tracks") {
      tracks = it.value().toArray();
      break;
    }
  }
  if (tracks.size() > 0) {
    for (int i = 0; i < tracks.size(); i++) {
      QJsonObject obj = tracks[i].toObject();
      if (!obj.isEmpty()) {
        Track track_t;
        track_t.index = obj.value("index").toDouble();
        track_t.title = obj.value("title").toString();
        track_t.length = obj.value("length").toDouble();
        list.append(track_t);
      }
    }
  }
  return list;
}

int DiscInfo::getReleaseYear() {
  QJsonValue val = value("date");
  if (val.isNull())
    return 0;

  QDate d = QDate::fromString(val.toString(), "yyyy-MM-dd");
  return (d.isValid() ? d.year() : 0);
}
