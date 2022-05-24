// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventoryproviders.h"
#include "antiqua_global.h"
//#include "applsettings.h"
//#include "myicontheme.h"
#include "buchfreund.h"
#include "providerstoolbar.h"
#include "providerstreeview.h"

#include <QDebug>
#include <QSizePolicy>
#include <QStringList>
#include <QVBoxLayout>

InventoryProviders::InventoryProviders(QWidget *parent) : Inventory{parent} {
  setObjectName("inventory_providers");
  setWindowTitle(tr("Providers") + "[*]");
  setClosable(false);

  p_providerList = QStringList({"Buchfreund", "ZVAB", "AbeBooks"});

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("inventory_providers_layout");
  layout->setContentsMargins(2, 2, 2, 2);

  m_splitter = new QSplitter(this);
  m_splitter->setObjectName("inventory_providers_splitter");
  m_splitter->setOrientation(Qt::Horizontal);
  layout->addWidget(m_splitter);

  m_pageView = new QTabWidget(m_splitter);
  m_pageView->setObjectName("providers_pages");
  m_pageView->setTabPosition(QTabWidget::South);
  m_splitter->insertWidget(0, m_pageView);

  m_listView = new ProvidersTreeView(m_splitter);
  m_listView->setObjectName("providers_payments_list");
  m_listView->setTreeViewHeaders();
  m_splitter->insertWidget(1, m_listView);

  m_toolBar = new ProvidersToolBar(this);
  layout->addWidget(m_toolBar);

  m_splitter->setStretchFactor(0, 70);
  m_splitter->setStretchFactor(1, 30);
  layout->setStretch(0, 1);
  setLayout(layout);

  m_listView->addProviders(p_providerList);
  m_listView->addOrder("Buchfreund", "BF-2249986");

  connect(m_toolBar, SIGNAL(s_createOrder()), this, SLOT(createEditOrders()));
  connect(m_listView, SIGNAL(s_queryOrder(const QString &, const QString &)),
          this, SLOT(queryOrder(const QString &, const QString &)));
}

bool InventoryProviders::addTab(QWidget *w) {
  QString identity = w->objectName();
  for (int i = 0; i < m_pageView->count(); i++) {
    if (m_pageView->widget(i)->objectName() == identity) {
      m_pageView->setCurrentIndex(i);
      return false;
    }
  }
  m_pageView->addTab(w, myIcon("edit_group"), identity);
  return true;
}

void InventoryProviders::searchConvert(const QString &search) {
  Q_UNUSED(search)
  /* im moment hier nicht notwendig */
}

void InventoryProviders::searchConvert() {
  /* im moment hier nicht notwendig */
}

void InventoryProviders::openTableView() {
  /* im moment hier nicht notwendig */
}

void InventoryProviders::queryOrder(const QString &provider,
                                    const QString &orderId) {
  if (!p_providerList.contains(provider))
    return;

  if (provider == "Buchfreund") {
    Buchfreund *bf = new Buchfreund(this);
    bf->setObjectName(orderId);
    bf->setWindowTitle(orderId);
    if (addTab(bf)) {
      bf->fetchOrderContent(orderId);
    }
    return;
  }

  qDebug() << Q_FUNC_INFO << provider << orderId;
}

void InventoryProviders::createEditCustomer(int cid) {
  m_toolBar->enableOrderButton(true);
  if (cid > 0) {
    customerId = cid;
    emit openEditCustomer(customerId);
  } else {
    customerId = -1;
  }
}

void InventoryProviders::createEditOrders() {
  qDebug() << Q_FUNC_INFO;
  if (customerId < 1)
    return;
}

void InventoryProviders::onEnterChanged() {
  qDebug() << Q_FUNC_INFO << "Aktuell Deaktiviert";
  return;
}
