// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DELIVERSERVICEBOX_H
#define DELIVERSERVICEBOX_H

#include <QComboBox>
#include <QWheelEvent>
#include <QWidget>

/**
 * @brief Auswahl der Paketzusteller
 * @ingroup DeliverService
 * Die Daten werden aus Tabelle "ref_delivery_service" gelesen.
 */
class DeliverServiceBox final : public QComboBox {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

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
