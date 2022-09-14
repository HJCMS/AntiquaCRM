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
    QJsonDocument doc = QJsonDocument::fromJson(data, &parser);
    if (parser.error == QJsonParseError::NoError) {
      QJsonObject obj = doc.object();
      QString value = obj.value("value").toString();
      if (value.isEmpty()) {
        qWarning("Empty client messages to socket!");
        return;
      }

      QString receiver = obj.value("receiver").toString();
      QString type = obj.value("type").toString().toUpper();

#ifdef ANTIQUA_DEVELOPEMENT
      qDebug() << Q_FUNC_INFO << obj;
#endif

      if (type == "SLOT" && receiver == "WINDOW") {
        if (value == "showAntiquaWindow") {
          emit showWindow();
          return;
        }
      }

      if (type == "NOTICE") {
        if (receiver.contains("SYSTEM"))
          emit sendSystemMessage(Antiqua::ErrorStatus::NOTICE, value);
        else
          emit sendStatusMessage(value);
      } else if (type == "WARNING") {
        if (receiver.contains("SYSTEM"))
          emit sendSystemMessage(Antiqua::ErrorStatus::WARNING, value);
        else
          emit sendStatusMessage(value);
      } else if (type == "FATAL") {
        if (receiver.contains("SYSTEM"))
          emit sendSystemMessage(Antiqua::ErrorStatus::FATAL, value);
        else
          emit sendStatusMessage(value);
      }
    } else {
      qWarning("Invalid Socket operation '%s';", qPrintable(data));
    }
  }
}

void SocketServer::incomingConnection(quintptr descriptor) {
  m_listener = new QLocalSocket(this);
  m_listener->setObjectName("local_socket_manager");
  m_listener->setSocketDescriptor(descriptor);
  readClientConnection();
}

const QString SocketServer::name() {
  QString name(ANTIQUACRM_CONNECTION_DOMAIN);
  name.append(".");
  name.append(QSysInfo::machineHostName());
  return name;
}
