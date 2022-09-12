// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "socketnotifier.h"

#include <QByteArray>
#include <QDataStream>
#include <QDebug>
#include <QSysInfo>
//#include <QVector>

SocketNotifier::SocketNotifier(QObject *parent) : QLocalServer{parent} {
  setObjectName("socket_notifier");
  setSocketOptions(QLocalServer::UserAccessOption);
  setMaxPendingConnections(100);
}

void SocketNotifier::incomingConnection(quintptr socketDescriptor) {
  m_listener = new QLocalSocket(this);
  m_listener->setObjectName("local_socket_manager");
  m_listener->setSocketDescriptor(socketDescriptor);
  if (m_listener->waitForReadyRead(timeout)) {
    QByteArray data = m_listener->readAll();
    QString msg = QString::fromLocal8Bit(data);
    qDebug() << Q_FUNC_INFO << data << msg;
    // emit statusMessage(msg);
  }
}

const QString SocketNotifier::name() {
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
