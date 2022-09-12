// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "socketserver.h"
#include "antiqua_global.h"

#include <QByteArray>
#include <QDataStream>
#include <QDebug>
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
    // qDebug() << Q_FUNC_INFO << "BytesRead:" << data;
    QString msg = QString::fromLocal8Bit(data);
    if (msg == "showAntiquaWindow") {
      emit showWindow();
      return;
    }
    // TODO
    emit statusMessage(msg);
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
