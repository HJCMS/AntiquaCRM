// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ATXSOCKET_H
#define ANTIQUACRM_ATXSOCKET_H

#include <QJsonObject>
#include <QLocalSocket>
#include <QObject>
#include <QStringList>

namespace AntiquaCRM {

/**
 * @brief Application Socket Transmitter
 * @section application
 * @example send a Window StatusBar Message
 * @code
 *  AntiquaCRM::ATxSocket messanger(this);
 *  messanger.setObjectName("transmitter_object_name");
 *  QJsonObject obj;
 *  obj.insert("window_status_message",QJsonValue("my Status Message"));
 *  messanger.pushMessage(obj);
 *  messanger.close();
 * @endcode
 */
class ATxSocket final : public QLocalSocket {
  Q_OBJECT

private:
  bool connected;
  qint8 timeout = 5; /**< Timeout in Seconds */

private Q_SLOTS:
  void getErrors(QLocalSocket::LocalSocketError error);
  void getState(QLocalSocket::LocalSocketState state);

public Q_SLOTS:
  /**
   * @brief push Operation via Json Object
   */
  void pushOperation(const QJsonObject &obj);

  /**
   * @brief Send a Window StatusBar Message
   */
  void pushStatusBarMessage(const QString &message);

public:
  explicit ATxSocket(QObject *parent = nullptr);

  /**
   * @brief list of accepted Json keys
   */
  const QStringList getOperations() const;

  /**
   * @brief close socket connection
   */
  void close() override;

  /**
   * @brief Socket serverName
   */
  static const QString antiquaServerName();
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_ATXSOCKET_H
