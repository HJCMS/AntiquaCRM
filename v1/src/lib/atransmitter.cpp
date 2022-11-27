// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "atransmitter.h"

#include <AGlobal>
#include <QDataStream>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonValue>
#include <QLocalServer>

namespace AntiquaCRM {

ATransmitter::ATransmitter(QObject *parent)
    : QLocalSocket{parent}, connected{false} {
  setServerName(antiquaSocketName());

  connect(this, SIGNAL(errorOccurred(QLocalSocket::LocalSocketError)),
          SLOT(getErrors(QLocalSocket::LocalSocketError)));
  connect(this, SIGNAL(stateChanged(QLocalSocket::LocalSocketState)),
          SLOT(getState(QLocalSocket::LocalSocketState)));
}

void ATransmitter::getErrors(QLocalSocket::LocalSocketError error) {
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

void ATransmitter::getState(QLocalSocket::LocalSocketState state) {
  QString verbose;
  switch (state) {
  case QLocalSocket::UnconnectedState:
    verbose = "Disconnected";
    connected = false;
    break;

  case QLocalSocket::ConnectedState:
    verbose = "Connected";
    connected = true;
    break;

  case QLocalSocket::ClosingState:
    verbose = "ClosingState";
    break;

  default:
    return;
  }

#ifdef ANTIQUA_DEVELOPEMENT
  qInfo("Transmitter::%s", qPrintable(verbose));
#endif
}

bool ATransmitter::pushOperation(const QJsonObject &obj) {
  if (!connected) {
    connectToServer(ANTIQUACRM_TRANSMITTER_MODE);
    if (!waitForConnected(ANTIQUACRM_SOCKET_TIMEOUT)) {
      qWarning("Socket Operation timeout!");
      return false;
    }
  }

  QByteArray data = QJsonDocument(obj).toJson(QJsonDocument::Compact);
  write(data, qstrlen(data));
  // Optional auf Windows testen...
  // return flush();
  return waitForBytesWritten(ANTIQUACRM_SOCKET_TIMEOUT);
}

bool ATransmitter::pushStatusBarMessage(const QString &message) {
  if (message.isEmpty())
    return false;

  QJsonObject obj;
  obj.insert("window_status_message", QJsonValue(message));
  return pushOperation(obj);
}

const QStringList ATransmitter::getOperations() const {
  QStringList l;
  l << "window_status_message";
  l << "window_operation";
  l << "plugin_operation";
  return l;
}

void ATransmitter::close() {
  if (ANTIQUACRM_TRANSMITTER_MODE == QIODevice::ReadWrite)
    disconnectFromServer();

  QLocalSocket::close();

  if (state() == QLocalSocket::UnconnectedState && connected) {
    connected = false;
  }
}

}; // namespace AntiquaCRM
