// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "socketserver.h"
#include "antiqua_global.h"

#include <QSysInfo>

SocketServer::SocketServer(QObject *parent) : QLocalServer{parent} {
  setObjectName("socket_server");
  setSocketOptions(QLocalServer::UserAccessOption);
  setMaxPendingConnections(1);
}

void SocketServer::incomingConnection(quintptr socketDescriptor) {
  qDebug() << Q_FUNC_INFO << "TODO" << socketDescriptor;
}

const QString SocketServer::name() {
  QString name(ANTIQUACRM_CONNECTION_PREFIX);
  name.append(ANTIQUACRM_NAME);
  name.append(".");
  name.append(QSysInfo::machineHostName());
  return name;
}
