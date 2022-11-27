// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ARECEIVER_H
#define ANTIQUACRM_ARECEIVER_H

#include <AGlobal>
#include <QJsonObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QObject>
#include <QString>

namespace AntiquaCRM {

class ANTIQUACRM_LIBRARY AReceiver final : public QLocalServer {
  Q_OBJECT

private:
  void createAction(const QJsonObject &obj);

private Q_SLOTS:
  void getTransmitterCaller();

Q_SIGNALS:
  void sendStatusMessage(const QString &message);
  void sendWindowOperation(const QJsonObject &obj);
  void sendPluginOperation(const QJsonObject &obj);

public:
  explicit AReceiver(QObject *parent = nullptr);
  virtual ~AReceiver();
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_ARECEIVER_H
