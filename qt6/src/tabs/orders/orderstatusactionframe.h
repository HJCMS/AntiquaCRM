// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_ORDER_STATUSACTIONFRAME_H
#define ANTIQUACRM_PLUGIN_ORDER_STATUSACTIONFRAME_H

#include <AntiquaWidgets>
#include <QFrame>
#include <QObject>

/**
 * @class OrderStatusActionFrame
 * @brief Set Orders Order-Status and Payment-Status
 *
 * This subclass control the user input of Order and Payment status.
 * Some status modi collidate with different order or payment status selection.
 * If an collidation exists.
 * A PopUp message will opened and the status resets to the last selection.
 */
class ANTIQUACRM_LIBRARY OrderStatusActionFrame final : public QFrame {
  Q_OBJECT
  Q_PROPERTY(AntiquaCRM::OrderStatus orderStatus READ getOrderStatus WRITE
                 setOrderStatus NOTIFY sendOrderStatus)
  Q_PROPERTY(AntiquaCRM::OrderPayment paymentStatus READ getOrderPayment WRITE
                 setOrderPayment NOTIFY sendOrderPayment)

private Q_SLOTS:
  void orderStatusChanged(AntiquaCRM::OrderStatus);
  void orderPaymentChanged(AntiquaCRM::OrderPayment);

Q_SIGNALS:
  /**
   * @brief The order status has been changed
   */
  void sendOrderStatus(AntiquaCRM::OrderStatus);

  /**
   * @brief The payment status has been changed
   */
  void sendOrderPayment(AntiquaCRM::OrderPayment);

  /**
   * @brief Sends a Message for Window PopUp Notification.
   */
  void sendNoticeMessage(const QString &);

  /**
   * @brief Sends a Status bar message
   */
  void sendNotifyStatus(const QString &);

public Q_SLOTS:
  /**
   * @brief set order status
   */
  void setOrderStatus(AntiquaCRM::OrderStatus);

  /**
   * @brief set payment status
   */
  void setOrderPayment(AntiquaCRM::OrderPayment);

public:
  /**
   * @brief Order status selecter widget
   * @note Must public for QObject::findChildren
   */
  AntiquaCRM::SelectOrderStatus *o_order_status;

  /**
   * @brief Payment status selecter widget
   * @note Must public for QObject::findChildren
   */
  AntiquaCRM::SelectOrderPayment *o_payment_status;

  /**
   * @param parent - parent object
   * @note do not forget to init signals after first fill
   */
  explicit OrderStatusActionFrame(QWidget *parent = nullptr);

  /**
   * @brief Input masks status of the order.
   *
   * Used to check whether the entry should be closed.
   *
   * Checks whether the input mask values are set as follows:
   * @li AInputWidget::o_payment_status == AntiquaCRM::OrderPayment::PAYED
   * @li AInputWidget::o_order_status == AntiquaCRM::OrderStatus::DELIVERED
   *
   * If both values are ok, true is returned.
   * @sa OrderStatusActionFrame::setStatusProtection
   */
  bool currentOrderStatus();

  /**
   * @brief get Protection status for current dialog.
   * @return true if protected otherwise false
   */
  bool isProtectable();

  /**
   * @brief Query order status
   */
  AntiquaCRM::OrderStatus getOrderStatus();

  /**
   * @brief Query payment status
   */
  AntiquaCRM::OrderPayment getOrderPayment();

  /**
   * @brief Initialize status and signals
   */
  void stepIn();

  /**
   * @brief Leave and block signals
   */
  void stepOut();
};

#endif // ANTIQUACRM_PLUGIN_ORDER_STATUSACTIONFRAME_H
