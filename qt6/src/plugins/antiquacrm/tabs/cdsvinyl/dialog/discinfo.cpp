// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "discinfo.h"

DiscInfo::DiscInfo(const QJsonObject &parent) : QJsonObject{parent} {}

const QString DiscInfo::trackTitle(const QJsonValue &value) const {
  QString str = value.toString();
  str.replace("'", "’");
  return str.trimmed();
}

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
  // Tracks
  if (tracks.size() > 0) {
    for (int i = 0; i < tracks.size(); i++) {
      QJsonObject obj = tracks[i].toObject();
      if (!obj.isEmpty()) {
        Track track_t;
        track_t.index = obj.value("position").toInt();
        track_t.title = trackTitle(obj.value("title"));
        list.append(track_t);
      }
    }
  }
  return list;
}

int DiscInfo::getReleaseYear() {
  const QRegularExpression yPattern("^\\d{4}$");
  QString val = value("date").toString().trimmed();
  if (val.contains(yPattern))
    return val.toInt();
  else if (!val.contains("-"))
    return 0;

  foreach (QString s, val.split("-")) {
    if (s.length() == 4) {
      return s.toInt();
    }
  }
  return 0;
}
