// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ORDERSHISTORYBUTTON_H
#define ANTIQUACRM_ORDERSHISTORYBUTTON_H

#include <QAction>
#include <QObject>
#include <QPushButton>
#include <QWidget>

/**
 * @brief Verschiedene abfragen Auswählen
 * @ingroup InventoryOrders
 * @class OrdersHistoryButton
 */
class OrdersHistoryButton : public QPushButton {
  Q_OBJECT

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
   * @brief Aufträge mit Lieferrung
   */
  QAction *ac_delivered;

  /**
   * @brief Aufträge mit Erinnerung gesendet
   */
  QAction *ac_remindet;

private Q_SLOTS:
  void setQueryClosedOrders();
  void setQueryNoPayments();
  void setQueryDelivered();
  void setQueryRemindet();

Q_SIGNALS:
  void sendDefaultView();
  void sendOrderStatusAction(int);
  void sendOrderPaymentAction(int);

public:
  explicit OrdersHistoryButton(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_ORDERSHISTORYBUTTON_H
