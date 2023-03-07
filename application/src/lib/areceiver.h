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

/**
 * @brief Application Socket Receiver
 * @ingroup core
 */
class ANTIQUACRM_LIBRARY AReceiver final : public QLocalServer {
  Q_OBJECT

private:
  /**
   * @brief Aufarbeitung der Signale
   */
  void createAction(const QJsonObject &obj);

private Q_SLOTS:
  /**
   * @brief Wenn eine Neue Verbindung aufgebaut wird.
   */
  void getTransmitterCaller();

Q_SIGNALS:
  /**
   * @brief Nachrichten an die Fensterleiste senden!
   */
  void sendInfoMessage(const QString &);
  void sendWarnMessage(const QString &);

  /**
   * @brief Sende Aktion an Registerkarte ...
   */
  void sendWindowOperation(const QJsonObject &);

  /**
   * @brief Send Aktion Plugins ...
   */
  void sendPluginOperation(const QJsonObject &);

public:
  explicit AReceiver(QObject *parent = nullptr);
  virtual ~AReceiver();
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_ARECEIVER_H
