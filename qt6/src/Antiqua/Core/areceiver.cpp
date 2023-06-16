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

  const QMap<QString, QMetaType> _operations = operations();
  const QString _action = obj.value("ACTION").toString();
  if (!_operations.contains(_action)) {
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
  if (!_target.contains("_tab")) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << "Invalid Socket operation Target!" << Qt::endl
             << "- Current:" << _target << Qt::endl
             << "Check AntiquaCRM::AReceiver::operations() to find permitted!";
#else
    qWarning("Unknown Socket operation target, aborted by policy rules!");
#endif
    return false;
  }

  // Check Meta-Type
  if (_operations.value(_action).id() != _value.metaType().id()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << "Invalid Socket operation Type!" << Qt::endl
             << "- Require:" << _operations.value(_action).id() << Qt::endl
             << "- Current:" << _value.metaType().id() << Qt::endl
             << "- Value:" << _value;
#else
    qWarning("Invalid Socket operation type, aborted by policy rules!");
#endif
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

const QMap<QString, QMetaType> AReceiver::operations() {
  QMap<QString, QMetaType> _m;
  const QMetaType _int64(QMetaType::LongLong);
  const QMetaType _string(QMetaType::QString);

  // Open order with orderId
  _m.insert("open_order", _int64);

  // Add Article to order with Article Id
  _m.insert("add_article", _int64);

  // Open Article with Article Id
  _m.insert("open_article", _int64);

  // Open Customer with Customer Id
  _m.insert("open_customer", _int64);

  // Create order action with customer Id
  _m.insert("create_order", _int64);

  // Send Customized Provider operation
  _m.insert("provider_update", _int64);

  // Messanger
  _m.insert("status_message", _string);
  _m.insert("popup_message", _string);

  return _m;
}

}; // namespace AntiquaCRM
