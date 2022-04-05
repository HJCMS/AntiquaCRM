// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "netjsonrequest.h"
#include "version.h"

#include <QtCore/QDebug>

NetJsonRequest::NetJsonRequest(QObject *parent)
    : QNetworkAccessManager{parent} {
  setObjectName("NetJsonRequest");

  connect(this, SIGNAL(finished()), this, SLOT(urlReplyFinished()));
}

void NetJsonRequest::replyFinished() { qDebug() << "TODO::replyFinished"; }

void NetJsonRequest::readReply() {
  qDebug() << "TODO::readReply";
  if (m_networkReply) {
    QString mimeType =
        m_networkReply->header(QNetworkRequest::ContentTypeHeader).toString();
    if (mimeType.contains("text/json")) {
        QByteArray data = m_networkReply->readAll();
        if ( data.isEmpty() )
          return;

    }
  }
}

void NetJsonRequest::queryJsonUrl(const QString &q) {
  m_networkReply = get(QNetworkRequest(QUrl(q)));
  connect(m_networkReply, SIGNAL(readyRead()), this, SLOT(readReply()));
}
