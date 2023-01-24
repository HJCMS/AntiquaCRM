// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "cddiscid.h"

#include <QDebug>
#include <QMetaObject>
#include <QTime>
#include <QUrlQuery>

// http://musicbrainz.org/doc/libdiscid
#include <discid/discid.h>

CDDiscId::CDDiscId(QObject *parent) : QThread{parent} {
  setObjectName("cd_info_handle");
}

bool CDDiscId::open() {
  DiscId *m_discId = discid_new();
  if (discid_read_sparse(m_discId, discid_get_default_device(), 0) == 0) {
    qWarning("DiscId: %s", discid_get_error_msg(m_discId));
    emit sendStatusMessage(tr("No Media found!"));
    return false;
  }

  // BEGIN MusicBrainz DiscID.
  p_brainz_id = QString::fromUtf8(discid_get_id(m_discId));
  p_submissionUrl = QUrl(discid_get_submission_url(m_discId));
  p_submissionUrl.setScheme("https");
  p_submissionUrl.setHost("musicbrainz.org");
  // END

  discid_free(m_discId);

  if (p_brainz_id.isEmpty() || !p_submissionUrl.isValid()) {
    emit sendStatusMessage(tr("No Disc Information found!"));
    return false;
  }

  // https://musicbrainz.org/doc/MusicBrainz_Entity
  QUrl url;
  url.setScheme("https");
  url.setHost("musicbrainz.org");
  url.setPath("/ws/2/discid/" + p_brainz_id);

  QUrlQuery url_query;
  url_query.addQueryItem("inc", "artists+recordings+genres");

  p_brainz_toc = QUrlQuery(p_submissionUrl).queryItemValue("toc");
  if (!p_brainz_toc.isEmpty())
    url_query.addQueryItem("toc", p_brainz_toc);

  url_query.addQueryItem("fmt", "json");
  url.setQuery(url_query);

  if (url.isValid())
    emit sendQueryDiscChanged(url);

  return url.isValid();
}

void CDDiscId::run() {
  if (open()) {
    qInfo("CDInfo search ...");
  } else {
    qWarning("CDInfo exited without result!");
    quit();
  }
}
