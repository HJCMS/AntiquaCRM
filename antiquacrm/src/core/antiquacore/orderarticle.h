// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ORDERARTICLE_ANTIQUACORE_H
#define ORDERARTICLE_ANTIQUACORE_H

#include "antiqua_global.h"

#include <QList>
#include <QMetaType>
#include <QVariant>

class ANTIQUACORE_EXPORT OrderArticle final {
private:
  int p_payment;
  int p_article;
  int p_orderid;
  int p_customer;
  int p_count;
  double p_price;
  double p_sell_price;
  QString p_title;
  QString p_summary;

public:
  explicit OrderArticle();

  void clear();

  inline const QString primaryIndex() { return "a_payment_id"; };

  /** a_payment_id */
  void setPayment(int);
  int payment();

  /** a_article_id */
  void setArticle(int);
  int article();

  /** a_order_id */
  void setOrder(int);
  int order();

  /** a_customer_id */
  void setCustomer(int);
  int customer();

  /** a_count */
  void setCount(int);
  int count();

  /** a_price */
  void setPrice(double);
  double price();

  /** a_sell_price */
  void setSellPrice(double);
  double sellPrice();

  /** a_title */
  void setTitle(const QString &);
  const QString title();

  void setSummary(const QString &);
  const QString summary();
};
Q_DECLARE_TYPEINFO(OrderArticle, Q_COMPLEX_TYPE);

#endif // ORDERARTICLE_ANTIQUACORE_H
