// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "version.h"
#include "network.h"

#include <QtCore/QDebug>

Network::Network(QObject *parent) : QNetworkAccessManager{parent} {
  setObjectName("Network");
}

void Network::replyFinished() { qDebug() << "TODO::replyFinished"; }

void Network::readReply() {
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

void Network::startRequest(const QUrl &url)
{
  m_networkReply = get(QNetworkRequest(url));
  connect(m_networkReply, SIGNAL(readyRead()), this, SLOT(readReply()));
}
