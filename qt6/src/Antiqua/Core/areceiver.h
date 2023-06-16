// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_ARECEIVER_H
#define ANTIQUACRM_ARECEIVER_H

#include <AGlobal>
#include <QJsonObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QMap>
#include <QMetaType>
#include <QObject>
#include <QString>

namespace AntiquaCRM {

/**
 * @class AReceiver
 * @brief This Socket Receiver is used in the Application Window class!
 *
 * This Local Socket Server receives all operation to AntiquaCRM::TabsIndex
 * classes, which add to AntiquaCRM::TabsWidget class. It will search for opened
 * Tab-Window and loaded plugins. If them exists and valid, it execute the
 * operation!
 *
 * The following structure is expected.
 * @code
 *  // Example: Open a Order from „inventory_orders“ to „orders_tab“.
 *  QJsonObject({
 *    // Command from Operations List
 *    "ACTION":"open_order",
 *    // Destination objectName(), the „_tab“ suffix is important.
 *    "TARGET":"orders_tab",
 *    // QVariant Value
 *    "VALUE":107
 *  });
 * @endcode
 *
 * @warning Some rules given for Parameter restrictions:
 * @li ACTION - Must in AReceiver::operations() list!
 * @li TARGET - Object/Destination name must exists or loaded!
 * @li VALUE  - Restricted to 256 chars!
 *
 * @ingroup CoreLibrary
 */
class ANTIQUACRM_LIBRARY AReceiver final : public QLocalServer {
  Q_OBJECT

private:
  /**
   * @brief prepare socket operation
   * @param obj - Json Object
   *
   * It check current parameters and prepare the Signal from given
   * JsonObject.
   */
  bool createAction(const QJsonObject &obj);

private Q_SLOTS:
  /**
   * @brief if a new connection is established.
   *
   * e.g. nextPendingConnection()
   */
  void getTransmitter();

Q_SIGNALS:
  /**
   * @brief Signal for Window StatusBar in Socket Listener!
   * @param message - notification message
   */
  void sendMessage(const QString &message);

  /**
   * @brief Operation signal to Socket Listener!
   * @param target - operation target
   * @param object - operation object
   */
  void sendOperation(const QString &target, const QJsonObject &object);

public:
  /**
   * @param parent - parent object
   */
  explicit AReceiver(QObject *parent = nullptr);
  virtual ~AReceiver();

  /**
   * @brief Usable operations.
   *
   * This static function present a list of all allowed socket operations!
   */
  static const QMap<QString, QMetaType> operations();
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_ARECEIVER_H
