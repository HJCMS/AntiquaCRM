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

public:
  enum HistoryQuery {
    FILTER_DEFAULT = 0,
    FILTER_NOT_PAID = 1,
    FILTER_DELIVERED_NOT_PAID = 2,
    FILTER_PAYMENT_REMINDED = 3,
    FILTER_COMPLETED = 4
  };
  explicit OrdersHistoryButton(QWidget *parent = nullptr);

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
  void setDefaultView();
  void setQueryClosedOrders();
  void setQueryNoPayments();
  void setQueryDelivered();
  void setQueryRemindet();

Q_SIGNALS:
  void sendDefaultView();
  void sendHistoryAction(int);
};

Q_DECLARE_METATYPE(OrdersHistoryButton::HistoryQuery);

#endif // ANTIQUACRM_ORDERSHISTORYBUTTON_H
