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
  int p_article;
  int p_orderid;
  int p_costumer;
  int p_count;
  double p_price;
  double p_sell_price;
  QString p_title;
  QString p_summary;

public:
  explicit OrderArticle();

  void setArticle(int);
  int article();

  void setOrder(int);
  int order();

  void setCostumer(int);
  int costumer();

  void setCount(int);
  int count();

  void setPrice(double);
  double price();

  void setSellPrice(double);
  double sellPrice();

  void setTitle(const QString &);
  const QString title();

  void setSummary(const QString &);
  const QString summary();
};
Q_DECLARE_TYPEINFO(OrderArticle, Q_COMPLEX_TYPE);

typedef QList<OrderArticle> OrderArticleList;
Q_DECLARE_METATYPE(OrderArticleList);

#endif // ORDERARTICLE_ANTIQUACORE_H
