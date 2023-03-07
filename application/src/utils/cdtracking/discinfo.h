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

/**
 * @brief The DiscInfo class
 * @ingroup AntiquaCDTracking
 */
class DiscInfo final : public QJsonObject {
private:
  const QString trackTitle(const QJsonValue &) const;

public:
  /**
   * @brief Track index
   * @ingroup AntiquaCDTracking
   */
  struct Track {
    int index;
    QString title;
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
