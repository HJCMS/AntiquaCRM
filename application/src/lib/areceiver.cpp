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
  connect(this, SIGNAL(newConnection()), SLOT(getTransmitterCaller()));
}

void AReceiver::createAction(const QJsonObject &obj) {
  if (obj.contains("window_status_message")) {
    QString message = obj.value("window_status_message").toString();
    message = message.trimmed();
    if (message.isEmpty() || message.length() > 256) {
      qWarning("Socket abort by policy rules!");
      return;
    }
    emit sendInfoMessage(message);
    return;
  } else if (obj.contains("window_warn_message")) {
    QString message = obj.value("window_warn_message").toString();
    message = message.trimmed();
    if (message.isEmpty() || message.length() > 256) {
      qWarning("Socket abort by policy rules!");
      return;
    }
    emit sendWarnMessage(message);
    return;
  }

  if (obj.contains("window_operation") && obj.contains("tab")) {
    emit sendWindowOperation(obj);
    return;
  }

  if (obj.contains("plugin_operation")) {
    emit sendPluginOperation(obj);
    return;
  }

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << "UNKNOWN OPERATION:" << obj;
#endif
}

void AReceiver::getTransmitterCaller() {
  QLocalSocket *m_caller = nextPendingConnection();
  if (m_caller == nullptr)
    return;

  connect(m_caller, SIGNAL(disconnected()), m_caller, SLOT(deleteLater()));

  if (m_caller->waitForReadyRead(ANTIQUACRM_SOCKET_TIMEOUT)) {
    QByteArray data = m_caller->readAll();
    QJsonParseError parser;
    QJsonDocument jdoc = QJsonDocument::fromJson(data, &parser);
    if (parser.error == QJsonParseError::NoError) {
      QJsonObject jobj = jdoc.object();
      if (jobj.isEmpty()) {
        qWarning("Socket abort by policy rules!");
        return;
      }
      createAction(jobj);
    } else {
      qWarning("Socketserver parse error: '%s'",
               qPrintable(parser.errorString()));
    }
  }
}

AReceiver::~AReceiver() {
  if(hasPendingConnections()) {
    qWarning("found pending connections");
  }

#ifdef ANTIQUA_DEVELOPEMENT
  qInfo("Shutdown and Close socket ...");
#endif
}

}; // namespace AntiquaCRM
