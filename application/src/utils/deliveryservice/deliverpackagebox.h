// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DELIVERPACKAGEBOX_H
#define DELIVERPACKAGEBOX_H

#include <QComboBox>
#include <QWheelEvent>
#include <QWidget>
#include <AntiquaCRM>

/**
 * @ingroup _deliveryservice
 * @brief Auswahl der Paketzusteller Angebote
 * Die Daten werden aus Tabelle "ref_delivery_cost" gelesen.
 */
class DeliverPackageBox final : public QComboBox {
  Q_OBJECT

private:
  AntiquaCRM::ASqlCore *m_sql;

protected:
  /**
   * @brief Ungewollte fehlerhafte Eingabe verhindern.
   */
  void wheelEvent(QWheelEvent *) override {};

public Q_SLOTS:
  /**
   * @brief Setze Paket Gruppe von Dienstleister
   * Suche Daten mit FOREIGN KEY ("delivery_id") (d_srv)
   */
  void setCurrentPackages(int srv);

Q_SIGNALS:
  /**
   * @brief Eine gültige Dienstleister Paketliste wurde ausgewählt.
   */
  void validServiceChanged(bool);

public:
  explicit DeliverPackageBox(QWidget *parent = nullptr);

  /**
   * @brief Datenbank Packet Nummer "d_cid" setzen.
   */
  void setCurrentPackageId(int cid);

  /**
   * @brief Aktuelle Datenbank Packet Nummer "d_cid"
   */
  int getCurrentPackageId();

  /**
   * @brief Paket Preis "d_price"
   */
  qreal getPackagePrice(int cid);

  /**
   * @brief International oder National "d_international"
   */
  bool isInternational();
};

#endif // DELIVERPACKAGEBOX_H
