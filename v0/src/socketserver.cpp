// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "socketserver.h"
#include "antiqua_global.h"

#include <QByteArray>
#include <QDataStream>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QSysInfo>
#include <QVector>

SocketServer::SocketServer(QObject *parent) : QLocalServer{parent} {
  setObjectName("socket_server");
  setSocketOptions(QLocalServer::UserAccessOption);
  setMaxPendingConnections(100);
}

void SocketServer::readClientConnection() {
  if (m_listener->waitForReadyRead(timeout)) {
    QByteArray data = m_listener->readAll();
    QJsonParseError parser;

    /**
     *  @short Erwartet folgende Struktur
     *  @note Für "type" siehe Antiqua::ErrorStatus
     *  QJsonObject {
     *    "receiver":"QString",
     *    "type":"QString",
     *    "value":"String"
     */
    QJsonDocument doc = QJsonDocument::fromJson(data, &parser);
    if (parser.error == QJsonParseError::NoError) {
      QJsonObject obj = doc.object();
      // SIGNAL => SLOT Verarbeitung
      if (obj.value("type").toString() == "SLOT") {
        QString receiver = obj.value("receiver").toString();
        QString value = obj.value("value").toString();
        if (receiver.contains("Window") && value == "showAntiquaWindow") {
          emit showWindow();
          return;
        }
      }
      // NOTICE, WARNING, FATAL
      if (obj.value("type").toString() == "NOTICE") {
        QString value = obj.value("value").toString();
        if (!value.isEmpty())
          emit statusMessage(Antiqua::ErrorStatus::NOTICE, value);
      } else if (obj.value("type").toString() == "WARNING") {
        QString value = obj.value("value").toString();
        if (!value.isEmpty())
          emit statusMessage(Antiqua::ErrorStatus::WARNING, value);
      } else if (obj.value("type").toString() == "FATAL") {
        QString value = obj.value("value").toString();
        if (!value.isEmpty())
          emit statusMessage(Antiqua::ErrorStatus::FATAL, value);
      }
    } else {
      qWarning("Invalid Socket operation '%s';", qPrintable(data));
    }
  }
}

void SocketServer::incomingConnection(quintptr socketDescriptor) {
  m_listener = new QLocalSocket(this);
  m_listener->setObjectName("local_socket_manager");
  m_listener->setSocketDescriptor(socketDescriptor);
  readClientConnection();
}

const QString SocketServer::name() {
  QString name(ANTIQUACRM_CONNECTION_DOMAIN);
  name.append(".");
  name.append(QSysInfo::machineHostName());
  return name;
}
