// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "atransmitter.h"
#include "autil.h"

#include <AGlobal>
#include <QDataStream>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonValue>
#include <QLocalServer>

namespace AntiquaCRM {

ATransmitter::ATransmitter(QObject *parent)
    : QLocalSocket{parent}, connected{false} {
  setServerName(AntiquaCRM::AUtil::socketName());

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
    qWarning("Unsupported Socket Operation Error");
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
#ifdef Q_OS_WIN
  return flush();
#else
  return waitForBytesWritten(ANTIQUACRM_SOCKET_TIMEOUT);
#endif
}

bool ATransmitter::pushStatusBarMessage(const QString &message) {
  if (message.isEmpty())
    return false;

  QJsonObject obj;
  obj.insert("ACTION", QJsonValue("status_message"));
  obj.insert("TARGET", QJsonValue("current_tab"));
  obj.insert("VALUE", QJsonValue(message));
  return pushOperation(obj);
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
