// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DELIVERSERVICEBOX_H
#define DELIVERSERVICEBOX_H

#include <QComboBox>
#include <QWheelEvent>
#include <QWidget>

/**
 * @ingroup _deliveryservice
 * @brief Auswahl der Paketzusteller
 * Die Daten werden aus Tabelle "ref_delivery_service" gelesen.
 */
class DeliverServiceBox final : public QComboBox {
  Q_OBJECT

protected:
  /**
   * @brief Prevent invalid input, large edit pages.
   */
  void wheelEvent(QWheelEvent *) override {};

public Q_SLOTS:
  /**
   * @brief Setze Service mit "d_id"
   */
  void setCurrentServiceId(int did);

public:
  explicit DeliverServiceBox(QWidget *parent = nullptr);
  /**
   * @brief load Delivery Services from SQL Database
   */
  void initDeliverServices();

  /**
   * @brief Aktuelle Datenbank Service Nummer "d_id"
   */
  int getCurrentServiceId();
};

#endif // DELIVERSERVICEBOX_H
