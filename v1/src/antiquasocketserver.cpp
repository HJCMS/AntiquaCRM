// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquasocketserver.h"

#include <QByteArray>
#include <QDataStream>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QSysInfo>

AntiquaSocketServer::AntiquaSocketServer(QObject *parent) : QLocalServer{parent} {
  setObjectName("socket_notifier");
  setSocketOptions(QLocalServer::UserAccessOption);
  setMaxPendingConnections(100);
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
      qDebug() << Q_FUNC_INFO << jdoc;
    } else {
      qDebug() << Q_FUNC_INFO << parser.errorString();
    }
  }
}

const QString AntiquaSocketServer::name() {
  QString name("de.hjcms.antiquacrm");
  name.append(".");
  name.append(QSysInfo::machineHostName());
  QString userName = qEnvironmentVariable("USER");
  if (!userName.isEmpty()) {
    name.append(".");
    name.append(userName);
  }
  return name;
}
