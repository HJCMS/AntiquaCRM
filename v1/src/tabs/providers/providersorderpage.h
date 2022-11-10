// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PROVIDERSORDERPAGE_H
#define ANTIQUACRM_PROVIDERSORDERPAGE_H

#include <AntiquaCRM>
#include <QObject>
#include <QJsonObject>
#include <QTextEdit>
#include <QWidget>

class ProviderPurchaseHeader;
class ProvidersPageView;
class ProviderPurchaseTable;
class ProviderPurchaseBar;
class ProviderBuyerInfo;
class ProviderOrderInfo;

class ProvidersOrderPage final : public QWidget {
  Q_OBJECT

private:
  Q_DISABLE_COPY(ProvidersOrderPage);
  AntiquaCRM::ASqlCore *m_sql;
  const QJsonObject p_order;
  ProviderPurchaseHeader *m_header;
  ProvidersPageView *m_tab;
  ProviderPurchaseTable *m_table;
  ProviderPurchaseBar *m_actionBar;

  ProviderBuyerInfo *m_buyerInfo;
  ProviderOrderInfo *m_orderInfo;
  QTextEdit *m_buyerComment;

  inline void pushCmd(const QJsonObject &action);

  bool findCustomer(const QJsonObject &customer);

private Q_SLOTS:
  void openOrder(qint64 oid);
  void openArticle(qint64 aid);
  void findArticleIds();
  void setCreateOrder();

public:
  explicit ProvidersOrderPage(const QJsonObject &order,
                              QWidget *parent = nullptr);
  bool loadOrderDataset();
};

#endif // ANTIQUACRM_PROVIDERSORDERPAGE_H
