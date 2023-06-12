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
  if (obj.contains("POSTMESSAGE")) {
    const QString _msg = obj.value("POSTMESSAGE").toString();
    const QString _type = obj.value("TYPE").toString();
    if (_msg.isEmpty() || _type.isEmpty() || _msg.length() > 256) {
      qWarning("Socket action aborted by policy rules!");
      return;
    }

    if (_type.contains("WARNING"))
      emit sendWarnMessage(_msg);
    else
      emit sendInfoMessage(_msg);

    return;
  }
  if (obj.contains("OPERATION")) {
    const QString _operation = obj.value("OPERATION").toString();
    if (_operation == "tab") {
      emit sendWindowOperation(obj);
      return;
    } else if (_operation == "provider") {
      emit sendPluginOperation(obj);
      return;
    }
  }

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << "UNKNOWN OPERATION:" << obj;
#endif
}

void AReceiver::getTransmitterCaller() {
  QLocalSocket *m_ls = nextPendingConnection();
  if (m_ls == nullptr) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "no local socket exists!";
#endif
    return;
  }

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
      createAction(_obj);
    } else {
      qWarning("Socketserver parse error: '%s'",
               qPrintable(_parser.errorString()));
    }
  }
}

AReceiver::~AReceiver() {
  if (hasPendingConnections()) {
    qWarning("found pending connections");
  }

#ifdef ANTIQUA_DEVELOPEMENT
  qInfo("Shutdown and Close socket ...");
#endif
}

}; // namespace AntiquaCRM
