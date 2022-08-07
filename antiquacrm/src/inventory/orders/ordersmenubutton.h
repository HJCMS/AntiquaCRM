// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ORDERSMENUBUTTON_ORDERS_H
#define ORDERSMENUBUTTON_ORDERS_H

#include <QAction>
#include <QObject>
#include <QPushButton>
#include <QWidget>

/**
 * @brief Verschiedene abfragen Auswählen
 * @ingroup InventoryOrders
 * @class OrdersMenuButton
 */
class OrdersMenuButton final : public QPushButton {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
   * @brief Standard Ansicht
   */
  QAction *ac_default;

  /**
   * @brief Alle geschlossenen Aufträge anzeigen.
   */
  QAction *ac_hasClosed;

  /**
   * @brief Aufträge ohne Bezahlung
   */
  QAction *ac_noPayment;

  /**
   * @brief Aufträge mit Lieferrung ausstehend.
   */
  QAction *ac_noDeliver;

private Q_SLOTS:
  void setQueryClosedOrders();
  void setQueryNoPayments();
  void setQueryNoDelivery();

Q_SIGNALS:
  void sendDefaultView();
  void sendCustomQuery(const QString &query);

public:
  explicit OrdersMenuButton(QWidget *parent = nullptr);
};

#endif // ORDERSMENUBUTTON_ORDERS_H
