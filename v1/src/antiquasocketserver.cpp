// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquasocketserver.h"

#include <AntiquaCRM>
#include <QByteArray>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonParseError>

AntiquaSocketServer::AntiquaSocketServer(QObject *parent)
    : QLocalServer{parent} {
  setObjectName("socket_notifier");
  setSocketOptions(QLocalServer::UserAccessOption);
  setMaxPendingConnections(100);
}

void AntiquaSocketServer::createAction(const QJsonObject &obj) {
  if (obj.contains("window_status_message")) {
    QString message = obj.value("window_status_message").toString();
    message = message.trimmed();
    if (message.isEmpty() || message.length() > 256) {
      qWarning("Socket abort by policy rules!");
      return;
    }
    emit sendStatusMessage(message);
    return;
  } else if (obj.contains("plugin_article_update")) {
    QJsonObject action = obj.value("plugin_article_update").toObject();
    if (action.isEmpty()) {
      qWarning("Socket abort by policy rules!");
      return;
    }
    emit sendOperation(obj);
    return;
  }
#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << "Unknown Socket Operation:" << obj;
#endif
}

void AntiquaSocketServer::incomingConnection(quintptr socketDescriptor) {
  m_listener = new QLocalSocket(this);
  m_listener->setObjectName("local_socket_manager");
  m_listener->setSocketDescriptor(socketDescriptor);
  if (m_listener->waitForReadyRead(timeout)) {
    QByteArray data = m_listener->readAll();
    QJsonParseError parser;
    QJsonDocument jdoc = QJsonDocument::fromJson(data, &parser);
    if (parser.error == QJsonParseError::NoError) {
      QJsonObject jobj = jdoc.object();
      if (jobj.isEmpty()) {
        qWarning("Socket abort by policy rules!");
        return;
      }
      createAction(jobj);
    } else {
      qWarning("Socketserver parse error: '%s'",
               qPrintable(parser.errorString()));
    }
  }
}

const QString AntiquaSocketServer::socketPath() {
  return AntiquaCRM::AStatusMessanger::antiquaServerName();
}
