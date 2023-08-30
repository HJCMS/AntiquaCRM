// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "areceiver.h"

#include <AntiquaCRM>
#include <QByteArray>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonParseError>

namespace AntiquaCRM {

AReceiver::AReceiver(QObject *parent) : QLocalServer{parent} {
  setObjectName("socket_notifier");
  setSocketOptions(QLocalServer::UserAccessOption);
  setMaxPendingConnections(100);
  connect(this, SIGNAL(newConnection()), SLOT(getTransmitter()));
}

AReceiver::~AReceiver() {
  if (hasPendingConnections()) {
    qWarning("found pending connections");
  }

#ifdef ANTIQUA_DEVELOPEMENT
  qInfo("Shutdown and Close socket ...");
#endif
}

bool AReceiver::createAction(const QJsonObject &obj) {
  if (!obj.contains("ACTION") || !obj.contains("VALUE")) {
    qInfo("Socket action aborted by policy rules!");
    return false;
  }

  const QString _action = obj.value("ACTION").toString();
  if (_action.isEmpty()) {
    qWarning("Unknown Socket action, aborted by policy rules!");
    return false;
  }

  // import value to QVariant
  const QVariant _value = obj.value("VALUE").toVariant();

  // Popup and Status messages
  if (_action.contains("_message")) {
    // meta check
    if (_value.metaType().id() != QMetaType::QString) {
      qWarning("Socket action aborted, invalid message body!");
      return false;
    }

    const QString _msg = _value.toString();
    if (_msg.isEmpty() || _msg.length() > 256) {
      qWarning("Socket action aborted by policy rules!");
      return false;
    }

    if (_action != "status_message")
      qInfo("Socketinfo: %s", qPrintable(_msg));

    emit sendMessage(_msg);
    return true;
  }

  // Check Destination suffix
  const QString _target = obj.value("TARGET").toString();

  if (_action.startsWith("providers") && _target.startsWith("plugin")) {
    // Plugin Operations
    if (!obj.contains("NAME") || obj.value("NAME").toString().isEmpty()) {
      qWarning("Unknown plugin name for Socket action!");
      return false;
    }
  } else if (!_target.contains("_tab")) {
    // Tabs Operations
    qWarning("Unknown Socket target, aborted by policy rules!");
    return false;
  }

  emit sendOperation(_target, obj);
  return true;
}

void AReceiver::getTransmitter() {
  QLocalSocket *m_ls = nextPendingConnection();
  if (m_ls == nullptr)
    return;

  connect(m_ls, SIGNAL(disconnected()), m_ls, SLOT(deleteLater()));

  if (m_ls->waitForReadyRead(ANTIQUACRM_SOCKET_TIMEOUT)) {
    QByteArray _data = m_ls->readAll();
    QJsonParseError _parser;
    QJsonDocument _doc = QJsonDocument::fromJson(_data, &_parser);
    if (_parser.error == QJsonParseError::NoError) {
      QJsonObject _obj = _doc.object();
      if (_obj.isEmpty()) {
        qWarning("Socket abort by policy rules!");
        return;
      }

      if (!createAction(_obj)) {
#ifdef ANTIQUA_DEVELOPEMENT
        qDebug() << Q_FUNC_INFO << _obj;
#else
        qWarning("Operation rejected!");
#endif
      }
    } else {
      qWarning("Socketserver parse error: '%s'",
               qPrintable(_parser.errorString()));
    }
  }
}

}; // namespace AntiquaCRM
