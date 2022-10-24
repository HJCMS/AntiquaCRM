// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_STATUSMESSANGER_H
#define ANTIQUACRM_STATUSMESSANGER_H

#include <QJsonObject>
#include <QLocalSocket>
#include <QObject>
#include <QStringList>

namespace AntiquaCRM {

/**
 * @brief Application System Messanger
 * @section application
 * @example send a Window StatusBar Message
 * @code
 *  AntiquaCRM::AStatusMessanger messanger(this);
 *  messanger.setObjectName("transmitter_object_name");
 *  QJsonObject obj;
 *  obj.insert("window_status_message",QJsonValue("my Status Message"));
 *  messanger.pushMessage(obj);
 *  messanger.close();
 * @endcode
 */
class AStatusMessanger final : public QLocalSocket {
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
  void pushMessage(const QJsonObject &obj);

  /**
   * @brief Send a Window StatusBar Message
   */
  void pushStatusBarMessage(const QString &message);

public:
  explicit AStatusMessanger(QObject *parent = nullptr);

  /**
   * @brief list of accepted Json keys
   */
  const QStringList pushOperations() const;

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

#endif // ANTIQUACRM_STATUSMESSANGER_H
