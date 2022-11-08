// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "providersorderpage.h"
#include "providerpurchaseactionbar.h"
#include "providerpurchasetable.h"

#include <QDebug>
#include <QLayout>
#include <QReadWriteLock>

ProvidersOrderPage::ProvidersOrderPage(const QJsonObject &order,
                                       QWidget *parent)
    : QWidget{parent}, p_order{order} {
  setWindowTitle(p_order.value("orderid").toString());
  setContentsMargins(0, 0, 0, 0);

  m_sql = new AntiquaCRM::ASqlCore(this);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 2, 0, 2);
  // Articles
  m_table = new ProviderPurchaseTable(this);
  layout->addWidget(m_table);

  m_actionBar = new ProviderPurchaseActionBar(this);
  layout->addWidget(m_actionBar);

  // TODO m_actionBar->setCustomer(provider, customerId);

  setLayout(layout);

  connect(m_table, SIGNAL(sendCheckArticles()), SLOT(findArticleIds()));
  connect(m_table, SIGNAL(sendOpenArticle(qint64)),
          SIGNAL(sendOpenArticle(qint64)));
  connect(m_actionBar, SIGNAL(sendCheckArticles()), SLOT(findArticleIds()));
}

void ProvidersOrderPage::findArticleIds() {
  QStringList l = m_table->getArticleIds();
  QString set;
  if (l.size() > 0) {
    set = "'" + l.join("','") + "'";
  }
  QString inSet("id IN (" + set + "'");
  // Es muss von allen Tabellen die Article Nummer geprüft werden!
  qDebug() << Q_FUNC_INFO << "TODO" << inSet;

}

bool ProvidersOrderPage::loadOrderDataset() {
  if (p_order.count() == 0)
    return false;

  QReadWriteLock locker;
  locker.lockForWrite();

  QJsonArray array = p_order.value("articles").toArray();
  m_table->setRowCount(array.count());
  int rows = m_table->rowCount();
  for (int r = 0; r < array.count(); r++) {
    QJsonObject o = array[r].toObject();
    m_table->setItem(r, 0, m_table->createItem(o.value("a_provider_id")));
    m_table->setItem(r, 1, m_table->createItem(o.value("a_article_id")));
    m_table->setItem(r, 2, m_table->createItem(o.value("a_count")));
    m_table->setItem(r, 3, m_table->createItem(o.value("a_price")));
    m_table->setItem(r, 4, m_table->createItem(o.value("a_title")));
  }
  // qDebug() << Q_FUNC_INFO << p_order.value("provider");
  locker.unlock();
  return true;
}
