// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "atxsocket.h"

#include <AGlobal>
#include <QJsonDocument>
#include <QJsonValue>
#include <QLocalServer>

namespace AntiquaCRM {

ATxSocket::ATxSocket(QObject *parent) : QLocalSocket{parent}, connected{false} {
  setServerName(antiquaSocketName());

  connect(this, SIGNAL(errorOccurred(QLocalSocket::LocalSocketError)),
          SLOT(getErrors(QLocalSocket::LocalSocketError)));
  connect(this, SIGNAL(stateChanged(QLocalSocket::LocalSocketState)),
          SLOT(getState(QLocalSocket::LocalSocketState)));
}

void ATxSocket::getErrors(QLocalSocket::LocalSocketError error) {
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

void ATxSocket::getState(QLocalSocket::LocalSocketState state) {
  switch (state) {
  case QLocalSocket::UnconnectedState:
    // qDebug() << "ATxSocket::Disconnected" << fullServerName();
    connected = false;
    return;

  case QLocalSocket::ConnectedState:
    // qDebug() << "ATxSocket::Connected" << fullServerName();
    connected = true;
    return;

  default:
    return;
  }
}

void ATxSocket::pushOperation(const QJsonObject &obj) {
  if (!connected)
    connectToServer(QIODevice::ReadWrite);

  QByteArray json = QJsonDocument(obj).toJson(QJsonDocument::Compact);
  write(json);
  waitForBytesWritten((timeout * 1000));
}

void ATxSocket::pushStatusBarMessage(const QString &message) {
  if (message.isEmpty())
    return;

  QJsonObject obj;
  obj.insert("window_status_message", QJsonValue(message));
  return pushOperation(obj);
}

const QStringList ATxSocket::getOperations() const {
  QStringList l;
  l << "window_status_message";
  l << "window_operation";
  l << "plugin_operation";
  return l;
}

void ATxSocket::close() {
  disconnectFromServer();
  if (state() == QLocalSocket::UnconnectedState || waitForDisconnected(1000)) {
    connected = false;
  }
}

}; // namespace AntiquaCRM
