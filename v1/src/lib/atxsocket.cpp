// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "atxsocket.h"

#include <AGlobal>
#include <QJsonDocument>
#include <QJsonValue>
#include <QLocalServer>

#ifndef ANTIQUA_SOCKET_TIMEOUT
#define ANTIQUA_SOCKET_TIMEOUT 5000
#endif

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
    qInfo("ATxSocket::Disconnected");
    connected = false;
    return;

  case QLocalSocket::ConnectedState:
    qInfo("ATxSocket::Connected");
    connected = true;
    return;

  case QLocalSocket::ClosingState:
    qInfo("ATxSocket::ClosingState");
    return;

  default:
    return;
  }
}

bool ATxSocket::pushOperation(const QJsonObject &obj) {
  if (!connected) {
    connectToServer(QIODevice::ReadWrite);
    if (waitForConnected(ANTIQUA_SOCKET_TIMEOUT)) {
      qInfo("Connected to Socket");
    } else {
      qWarning("Socket Operation timeout!");
      return false;
    }
  }

  QByteArray json = QJsonDocument(obj).toJson(QJsonDocument::Compact);
  write(json, qstrlen(json));
  return waitForBytesWritten(ANTIQUA_SOCKET_TIMEOUT);
}

bool ATxSocket::pushStatusBarMessage(const QString &message) {
  if (message.isEmpty())
    return false;

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
#ifndef Q_OS_WIN
  disconnectFromServer();
  if (state() == QLocalSocket::UnconnectedState && connected) {
    connected = false;
  }
#endif
}

}; // namespace AntiquaCRM
