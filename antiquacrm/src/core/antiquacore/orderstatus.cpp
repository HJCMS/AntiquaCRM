#include "orderstatus.h"

#include <QObject>

OrderStatus::OrderStatus() {
  p_index = -1;
  p_title = QString();
}

OrderStatus::OrderStatus(int index, const QString &title) {
  p_index = index;
  p_title = title;
}

void OrderStatus::setIndex(int index) { p_index = index; }

int OrderStatus::index() { return p_index; }

void OrderStatus::setTitle(const QString &title) { p_title = title; }

const QString OrderStatus::title() { return p_title; }

OrderStatusList::OrderStatusList() : p_list() {
  p_list.clear();
  p_list.append(OrderStatus(0, QObject::tr("open order")));  /**< offene Bestellung */
  p_list.append(OrderStatus(1, QObject::tr("order start"))); /**< Auftragsbeginn */
  p_list.append(OrderStatus(2, QObject::tr("package picked up"))); /**< Paket abgeholt */
  p_list.append(OrderStatus(3, QObject::tr("package delivery"))); /**< Paketzustellung */
  p_list.append(OrderStatus(4, QObject::tr("package delivered"))); /**< Paket geliefert */
  p_list.append(OrderStatus(5, QObject::tr("delivery completed"))); /**< Lieferung abgeschlossen */
}

int OrderStatusList::size() { return p_list.size(); };

const OrderStatus OrderStatusList::status(int index) {
  OrderStatus ret(p_list.at(0));
  for (int i = 0; i < p_list.size(); i++) {
    OrderStatus cur = p_list.at(i);
    if (cur.index() == index)
      return cur;
  }
  return ret;
}

const int OrderStatusList::index(int index) {
  OrderStatus ret = status(index);
  return ret.index();
}

const QString OrderStatusList::title(int index) {
  OrderStatus ret = status(index);
  return ret.title();
}
