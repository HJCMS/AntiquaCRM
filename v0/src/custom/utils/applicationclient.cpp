// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "applicationclient.h"
#include "antiqua_global.h"

#include <QByteArray>
#include <QDataStream>
#include <QIODevice>
#include <QSysInfo>

static const QString socketName() {
  QString name(ANTIQUACRM_CONNECTION_DOMAIN);
  name.append(".");
  name.append(QSysInfo::machineHostName());
  return name;
}

ApplicationClient::ApplicationClient(QObject *parent) : QLocalSocket{parent} {
  setObjectName("socket_client_messanger");
  setServerName(socketName());

  connect(this, SIGNAL(errorOccurred(QLocalSocket::LocalSocketError)), this,
          SLOT(getErrors(QLocalSocket::LocalSocketError)));
  connect(this, SIGNAL(stateChanged(QLocalSocket::LocalSocketState)), this,
          SLOT(getState(QLocalSocket::LocalSocketState)));
}

void ApplicationClient::initConnection() {
  connectToServer(QIODevice::ReadWrite);
}

void ApplicationClient::writeToServer() {
  if (!isConnected || p_message_buffer.isEmpty())
    return;

  QByteArray data(p_message_buffer.toLocal8Bit());
  write(data);
  waitForBytesWritten(p_timeout);
}

void ApplicationClient::getErrors(QLocalSocket::LocalSocketError error) {
  switch (error) {
  case QLocalSocket::ConnectionRefusedError:
    qWarning("ConnectionRefusedError");
    return;

  case QLocalSocket::PeerClosedError:
    qWarning("PeerClosedError");
    return;

  case QLocalSocket::ServerNotFoundError:
    qWarning("ServerNotFoundError");
    return;

  case QLocalSocket::SocketAccessError:
    qWarning("SocketAccessError");
    return;

  case QLocalSocket::SocketResourceError:
    qWarning("SocketResourceError");
    return;

  case QLocalSocket::SocketTimeoutError:
    qWarning("SocketTimeoutError");
    return;

  case QLocalSocket::DatagramTooLargeError:
    qWarning("DatagramTooLargeError");
    return;

  case QLocalSocket::ConnectionError:
    qWarning("ConnectionError");
    return;

  case QLocalSocket::UnsupportedSocketOperationError:
    qWarning("UnsupportedSocketOperationError");
    return;

  case QLocalSocket::OperationError:
    qWarning("OperationError");
    return;

  default:
    qWarning("UnknownSocketError");
  }
}

void ApplicationClient::getState(QLocalSocket::LocalSocketState state) {
  switch (state) {
  case QLocalSocket::UnconnectedState:
    isConnected = false;
    return;

  case QLocalSocket::ConnectedState:
    // qDebug() << "Connected:" << fullServerName();
    isConnected = true;
    return;

  default:
    return;
  }
}

void ApplicationClient::sendMessage(const QString &message) {
  p_message_buffer = message;
  if (!isConnected)
    initConnection();

  writeToServer();
}

ApplicationClient::~ApplicationClient() {}
