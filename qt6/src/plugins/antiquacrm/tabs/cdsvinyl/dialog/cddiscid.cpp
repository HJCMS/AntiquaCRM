// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "cddiscid.h"

#include <AGlobal>
#include <QDebug>
#include <QMetaObject>
#include <QRegularExpression>
#include <QTime>
#include <QUrlQuery>

// http://musicbrainz.org/doc/libdiscid
#include <discid/discid.h>

CDDiscId::CDDiscId(QObject *parent) : QThread{parent} {
  setObjectName("cd_info_handle");
}

bool CDDiscId::isBarcocde(const QString &ean) {
  if (ean.contains(QRegularExpression("^0{5,}")))
    return false; // invalid checksum

  QString gtin = ean.trimmed();
  int len = gtin.size();
  if ((len < 13 && len != 10) || (len != 13)) {
    if (len == 9) {
      gtin = ean.rightJustified(10, '0');
    } else if (len == 12) {
      gtin = ean.rightJustified(13, '0');
    } else {
      return false;
    }
  }

  int gtt = gtin.length(); // GTIN Types 10/13
  int apd = 0;             // A: Product digit
  int brd = 0;             // B: Roundet digit
  int ccd = 0;             // C: Checksum digit
  int pos = 0;             // Start position
  for (int i = gtt; i > 0; --i) {
    int num = gtin.at(i - 1).digitValue();
    if (i == gtt) {
      ccd = num;
      continue;
    }
    apd += (pos % 2 & 1) ? (num * 1) : (num * 3);
    pos++;
  }
  if (ccd == 0)
    return (apd % 10 == 0);

  brd = apd;
  for (int r = 0; r <= 9; ++r) {
    if (++brd % 10 == 0)
      break;
  }
  return ((brd - apd) == ccd);
}

bool CDDiscId::open() {
  DiscId *m_discId = discid_new();
  if (discid_read_sparse(m_discId, discid_get_default_device(),
                         DISCID_FEATURE_MCN) == 0) {
    qWarning("DiscId: %s", discid_get_error_msg(m_discId));
    emit sendStatusMessage(tr("No Media found!"));
    return false;
  }

  // BEGIN MusicBrainz
  p_brainz_id = QString::fromUtf8(discid_get_id(m_discId));
  p_brainz_toc = QString::fromUtf8(discid_get_toc_string(m_discId));
  // END

  // BEGIN FreeDB
  p_cddb_id = QString::fromUtf8(discid_get_freedb_id(m_discId));
  int tracks = discid_get_last_track_num(m_discId);
  p_cddb_toc = QString::number(tracks);
  for (int i = discid_get_first_track_num(m_discId); i < tracks; i++) {
    int offset = discid_get_track_offset(m_discId, i);
    p_cddb_toc += "+" + QString::number(offset);
  }
  // END

  // EAN/UPC/GTIN
  p_gtin = QString::fromUtf8(discid_get_mcn(m_discId));
  if (isBarcocde(p_gtin))
    emit sendBarcode(p_gtin);

  discid_free(m_discId);

  if (p_brainz_id.isEmpty()) {
    emit sendStatusMessage(tr("No Disc Information found!"));
    return false;
  }

  QUrl url = queryMusicBrainz();
  if (url.isValid()) {
    emit sendQuery(url);
    return true;
  }
  return false;
}

void CDDiscId::run() {
  qInfo("CDInfo started ...");
  if (!open()) {
    qWarning("CDInfo exited without result!");
    quit();
  }
}

const QUrl CDDiscId::queryMusicBrainz() {
  // https://musicbrainz.org/doc/MusicBrainz_Entity
  QUrl url;
  url.setScheme("https");
  url.setHost("musicbrainz.org");
  url.setPath("/ws/2/discid/" + p_brainz_id);

  QUrlQuery url_query;
  const QRegularExpression _numpattern("\\D");
  url_query.addQueryItem("inc", "artists+recordings");
  url_query.addQueryItem("toc", p_brainz_toc.replace(_numpattern, "+"));
  url_query.addQueryItem("fmt", "json");
  url.setQuery(url_query);
  return url;
}

const QUrl CDDiscId::queryCDDB() {
  QUrl url;
  url.setScheme("http");
  url.setHost("gnudb.gnudb.org");
  url.setPath("/~cddb/cddb.cgi");

  QStringList cmd("cddb");
  cmd.append("query");
  cmd.append(p_cddb_id);
  cmd.append(p_cddb_toc);

  QStringList ehlo("private");
  ehlo.append("free.the.cddb");
  ehlo.append(ANTIQUACRM_NAME);
  ehlo.append(ANTIQUACRM_VERSION);

  QUrlQuery url_query;
  url_query.addQueryItem("cmd", cmd.join("+"));
  url_query.addQueryItem("hello", ehlo.join("+"));
  url_query.addQueryItem("proto", "6");
  url.setQuery(url_query);
  return url;
}
