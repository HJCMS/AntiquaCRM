// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "workspace.h"
#include "antiqua_global.h"
#include "inventorybooks.h"
#include "inventorycustomers.h"
#include "inventoryorders.h"
#include "inventoryprints.h"
#include "inventoryproviders.h"
#include "inventoryviews.h"
#include "myicontheme.h"
#include "providerorders.h"

#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QLinearGradient>
#include <QMenu>
#include <QMetaObject>
#include <QTabBar>

WorkspaceTabBar::WorkspaceTabBar(QWidget *parent) : QTabBar{parent} {
  setMovable(true);
}

void WorkspaceTabBar::checkToClose() {
  if (index >= 0)
    emit s_closeTab(index);
}

void WorkspaceTabBar::tabInserted(int index) {
  setTabIcon(index, myIcon("tab"));
  QTabBar::tabInserted(index);
}

void WorkspaceTabBar::contextMenuEvent(QContextMenuEvent *ev) {
  index = tabAt(ev->pos());
  bool b = tabData(index).toBool();
  QMenu *m = new QMenu("TabAction", this);
  m->setEnabled(b);
  QAction *ac_close = m->addAction(myIcon("tab_remove"), tr("Close"));
  ac_close->setObjectName("ac_context_close_tab");
  connect(ac_close, SIGNAL(triggered()), this, SLOT(checkToClose()));
  m->exec(ev->globalPos());
  delete m;
}

Workspace::Workspace(QWidget *parent) : QTabWidget{parent} {
  setObjectName("WorkspaceTabWidget");
  setWindowTitle(tr("Workspace") + " [*]");
  setTabsClosable(false);
  setUsesScrollButtons(false);

  m_tabBar = new WorkspaceTabBar(tabBar());
  setTabBar(m_tabBar);

  connect(this, SIGNAL(tabCloseRequested(int)), SLOT(closeTabClicked(int)));
  connect(m_tabBar, SIGNAL(s_closeTab(int)), SLOT(closeTabClicked(int)));
  connect(m_tabBar, SIGNAL(tabBarClicked(int)), SLOT(tabViewClicked(int)));
}

int Workspace::addInventoryBooks(int index) {
  m_tabBooks = new InventoryBooks(this);
  // Zur Bestellung hinzufügen
  connect(m_tabBooks, SIGNAL(s_addArticleOrder(int)), this,
          SLOT(addArticleOrder(int)));
  // Änderungen aufzeichenen
  connect(m_tabBooks, SIGNAL(s_windowModified(bool)), this,
          SIGNAL(s_windowModified(bool)));
  // Nachrichten an Hauptfenster
  connect(m_tabBooks, SIGNAL(s_postMessage(const QString &)), this,
          SIGNAL(s_postMessage(const QString &)));
  // Artikel Bestandsänderung an Providers senden
  connect(m_tabBooks, SIGNAL(s_articleCount(int, int)), this,
          SLOT(updateArticleCount(int, int)));
  int i = insertTab(index, m_tabBooks, tr("Books"));
  m_tabBar->setTabData(i, m_tabBooks->isClosable());
  setTabToolTip(i, tr("Book Inventory"));
  setTabIcon(i, myIcon("toggle_log"));
  return i;
}

int Workspace::addInventoryPrints(int index) {
  m_tabPrints = new InventoryPrints(this);
  // Zur Bestellung hinzufügen
  connect(m_tabPrints, SIGNAL(s_addArticleOrder(int)), this,
          SLOT(addArticleOrder(int)));
  // Änderungen aufzeichenen
  connect(m_tabPrints, SIGNAL(s_windowModified(bool)), this,
          SIGNAL(s_windowModified(bool)));
  // Nachrichten an Hauptfenster
  connect(m_tabPrints, SIGNAL(s_postMessage(const QString &)), this,
          SIGNAL(s_postMessage(const QString &)));
  // Artikel Bestandsänderung an Providers senden
  connect(m_tabPrints, SIGNAL(s_articleCount(int, int)), this,
          SLOT(updateArticleCount(int, int)));
  int i = insertTab(index, m_tabPrints, tr("Prints"));
  m_tabBar->setTabData(i, m_tabPrints->isClosable());
  setTabToolTip(i, tr("Prints, Stitches and Photo inventory"));
  setTabIcon(i, myIcon("image"));
  return i;
}

int Workspace::addInventoryCustomers(int index) {
  m_tabCustomers = new InventoryCustomers(this);
  // Neue Bestellung anlegen
  connect(m_tabCustomers, SIGNAL(s_createOrder(int)), this,
          SLOT(createOrder(int)));
  // Änderungen aufzeichenen
  connect(m_tabCustomers, SIGNAL(s_windowModified(bool)), this,
          SIGNAL(s_windowModified(bool)));
  // Nachrichten an Hauptfenster
  connect(m_tabCustomers, SIGNAL(s_postMessage(const QString &)), this,
          SIGNAL(s_postMessage(const QString &)));
  int i = insertTab(index, m_tabCustomers, tr("Customers"));
  m_tabBar->setTabData(i, m_tabCustomers->isClosable());
  setTabToolTip(i, tr("Customers inventory"));
  setTabIcon(i, myIcon("edit_group"));
  return i;
}

int Workspace::addInventoryOrders(int index) {
  m_tabOrders = new InventoryOrders(this);
  // Änderungen aufzeichenen
  connect(m_tabOrders, SIGNAL(s_windowModified(bool)), this,
          SIGNAL(s_windowModified(bool)));
  // Nachrichten an Hauptfenster
  connect(m_tabOrders, SIGNAL(s_postMessage(const QString &)), this,
          SIGNAL(s_postMessage(const QString &)));
  // Artikel Bestandsänderung an Providers senden
  connect(m_tabOrders, SIGNAL(s_articleCount(int, int)), this,
          SLOT(updateArticleCount(int, int)));
  int i = insertTab(index, m_tabOrders, tr("Orders"));
  m_tabBar->setTabData(i, m_tabOrders->isClosable());
  setTabToolTip(i, tr("Order Inventory"));
  setTabIcon(i, myIcon("autostart"));
  return i;
}

int Workspace::addInventoryProviders(int index) {
  m_tabProviders = new InventoryProviders(this);
  // Änderunge aufzeichenen
  connect(m_tabProviders, SIGNAL(s_windowModified(bool)), this,
          SIGNAL(s_windowModified(bool)));
  // Nachrichten an Hauptfenster
  connect(m_tabProviders, SIGNAL(s_postMessage(const QString &)), this,
          SIGNAL(s_postMessage(const QString &)));
  // Kunden Editieren
  connect(m_tabProviders, SIGNAL(openEditCustomer(int)), this,
          SLOT(editCustomerEntry(int)));
  // Auftrag/Bestellung erstellen
  connect(m_tabProviders, SIGNAL(createOrder(const ProviderOrder &)), this,
          SLOT(createOrder(const ProviderOrder &)));
  // Einen Artikel öffnen
  connect(m_tabProviders, SIGNAL(openEditArticle(int, const QString &)), this,
          SLOT(editArticleEntry(int, const QString &)));
  int i = insertTab(index, m_tabProviders, tr("Ordering"));
  m_tabBar->setTabData(i, m_tabProviders->isClosable());
  setTabToolTip(i, tr("Providers ordering"));
  setTabIcon(i, myIcon("autostart"));
  return i;
}

int Workspace::addInventoryViews(int index) {
  m_tabViews = new InventoryViews(this);
  // Änderunge aufzeichenen
  connect(m_tabViews, SIGNAL(s_windowModified(bool)), this,
          SIGNAL(s_windowModified(bool)));
  // Nachrichten an Hauptfenster
  connect(m_tabViews, SIGNAL(s_postMessage(const QString &)), this,
          SIGNAL(s_postMessage(const QString &)));
  // Buch editieren
  connect(m_tabViews, SIGNAL(s_editBookEntry(int)), this,
          SLOT(editBookEntry(int)));
  // Kunden editieren
  connect(m_tabViews, SIGNAL(s_editCustomerEntry(int)), this,
          SLOT(editCustomerEntry(int)));
  // Drucke und Stiche editieren
  connect(m_tabViews, SIGNAL(s_editPrintsEntry(int)), this,
          SLOT(editPrintsEntry(int)));
  int i = insertTab(index, m_tabViews, tr("Views"));
  m_tabBar->setTabData(i, m_tabViews->isClosable());
  setTabToolTip(i, tr("Views"));
  setTabIcon(i, myIcon("search"));
  return i;
}

void Workspace::editBookEntry(int articleId) {
  if (articleId > 0 && indexOf(m_tabBooks) >= 0) {
    m_tabBooks->editBookEntry(articleId);
    setCurrentWidget(m_tabBooks);
  } else {
    emit s_postMessage(tr("Books tab isn't open!"));
  }
}

void Workspace::editPrintsEntry(int articleId) {
  if (articleId > 0 && indexOf(m_tabPrints) >= 0) {
    m_tabPrints->editPrintsEntry(articleId);
    setCurrentWidget(m_tabPrints);
  } else {
    emit s_postMessage(tr("Prints tab isn't open!"));
  }
}

void Workspace::editArticleEntry(int articleId, const QString &section) {
  // qDebug() << Q_FUNC_INFO << articleId << section;
  if (section == "PRINTS") {
    editPrintsEntry(articleId);
  } else if (section == "BOOKS") {
    editBookEntry(articleId);
  }
}

void Workspace::editCustomerEntry(int customerId) {
  if (customerId > 0 && indexOf(m_tabCustomers) >= 0) {
    m_tabCustomers->editCustomer(customerId);
    setCurrentWidget(m_tabCustomers);
  } else {
    emit s_postMessage(tr("Order tab isn't open!"));
  }
}

void Workspace::createOrder(int customerId) {
  if (customerId > 0 && indexOf(m_tabOrders) >= 0) {
    m_tabOrders->createOrder(customerId);
    setCurrentWidget(m_tabOrders);
  } else {
    emit s_postMessage(tr("Order tab isn't open!"));
  }
}

void Workspace::createOrder(const ProviderOrder &order) {
  if (indexOf(m_tabOrders) >= 0) {
    m_tabOrders->createOrder(order);
    setCurrentWidget(m_tabOrders);
  } else {
    emit s_postMessage(tr("Order tab isn't open!"));
  }
}

void Workspace::addArticleOrder(int articleId) {
  if (articleId > 0 && indexOf(m_tabOrders) >= 0) {
    bool b = m_tabOrders->addArticleToOrder(articleId);
    if (b) {
      setCurrentWidget(m_tabOrders);
    }
  } else {
    emit s_postMessage(tr("Order tab isn't open!"));
  }
}

void Workspace::updateArticleCount(int articleId, int count) {
  if (indexOf(m_tabProviders) >= 0) {
    // qDebug() << Q_FUNC_INFO << articleId << count;
    if (m_tabProviders->updateArticleCount(articleId, count)) {
      emit s_postMessage(tr("successfully"));
    } else {
      emit s_postMessage(tr("an error occurred"));
    }
  } else {
    emit s_postMessage(tr("Provider tab isn't open!"));
  }
}

void Workspace::closeTabClicked(int index) {
  Inventory *tab = qobject_cast<Inventory *>(widget(index));
  if (tab != nullptr && tab->isClosable()) {
    if (!tab->isWindowModified()) {
      removeTab(index);
      return;
    }
    emit s_postMessage(tr("Cant close this tab, unsafed changes!"));
  }
}

void Workspace::tabViewClicked(int i) {
  reinterpret_cast<Inventory *>(widget(i))->onEnterChanged();
}

void Workspace::tabRemoved(int index) {
  if (!isTabEnabled(index))
    delete widget(index);
}

void Workspace::openTab(int index) {
  int i = 0;
  if (index == Tab::Books) {
    if (indexOf(m_tabBooks) < 0) {
      i = addInventoryBooks(count() + 1);
    } else {
      setCurrentWidget(m_tabBooks);
      return;
    }
  } else if (index == Tab::Prints) {
    if (indexOf(m_tabPrints) < 0) {
      i = addInventoryPrints(count() + 1);
    } else {
      setCurrentWidget(m_tabPrints);
      return;
    }
  } else if (index == Tab::Customers) {
    if (indexOf(m_tabCustomers) < 0) {
      i = addInventoryCustomers(count() + 1);
    } else {
      setCurrentWidget(m_tabCustomers);
      return;
    }
  } else if (index == Tab::Orders) {
    if (indexOf(m_tabOrders) < 0) {
      i = addInventoryOrders(count() + 1);
    } else {
      setCurrentWidget(m_tabOrders);
      return;
    }
  } else if (index == Tab::Providers) {
    if (indexOf(m_tabProviders) < 0) {
      i = addInventoryProviders(count() + 1);
    } else {
      setCurrentWidget(m_tabProviders);
      return;
    }
  } else if (index == Tab::Views) {
    if (indexOf(m_tabViews) < 0) {
      i = addInventoryViews(count() + 1);
    } else {
      setCurrentWidget(m_tabViews);
      return;
    }
  }
  setCurrentIndex(i);
}
