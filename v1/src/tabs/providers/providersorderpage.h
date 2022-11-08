// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PROVIDERSORDERPAGE_H
#define ANTIQUACRM_PROVIDERSORDERPAGE_H

#include <AntiquaCRM>
#include <QObject>
#include <QJsonObject>
#include <QWidget>

class ProviderPurchaseTable;
class ProviderPurchaseActionBar;

class ProvidersOrderPage final : public QWidget {
  Q_OBJECT

private:
  Q_DISABLE_COPY(ProvidersOrderPage);
  AntiquaCRM::ASqlCore *m_sql;
  const QJsonObject p_order;
  ProviderPurchaseTable *m_table;
  ProviderPurchaseActionBar *m_actionBar;

private Q_SLOTS:
  void findArticleIds();

Q_SIGNALS:
  void sendCheckArticleIds(QList<qint64> &);
  void sendOpenArticle(qint64);

public:
  explicit ProvidersOrderPage(const QJsonObject &order,
                              QWidget *parent = nullptr);
  bool loadOrderDataset();
};

#endif // ANTIQUACRM_PROVIDERSORDERPAGE_H
