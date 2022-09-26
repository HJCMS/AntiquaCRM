// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "workspace.h"
#include "workspacetabbar.h"
#include "inventorybooks.h"
#include "inventorycustomers.h"
#include "inventoryorders.h"
#include "inventoryprints.h"
#include "inventoryproviders.h"
#include "inventoryviews.h"
#include "myicontheme.h"
#include "providerorders.h"

#include <QApplication>
#include <QDebug>

Workspace::Workspace(QWidget *parent) : QTabWidget{parent} {
  setObjectName("WorkspaceTabWidget");
  setWindowTitle(tr("Workspace") + " [*]");
  setTabPosition(QTabWidget::North);
  setTabsClosable(false);
  setUsesScrollButtons(false);

  m_tabBar = new WorkspaceTabBar(tabBar());
  setTabBar(m_tabBar);

  connect(this, SIGNAL(tabCloseRequested(int)), SLOT(closeTabClicked(int)));
  connect(m_tabBar, SIGNAL(sendCloseTab(int)), SLOT(closeTabClicked(int)));
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
  connect(m_tabBooks, SIGNAL(sendStatusBarMessage(const QString &)), this,
          SLOT(prepareMessanger(const QString &)));
  // Artikel Bestandsänderung an Providers senden
  connect(m_tabBooks, SIGNAL(s_articleCount(int, int)), this,
          SLOT(updateArticleCount(int, int)));
  // Bilder aus der Datenbank hochladen
  connect(m_tabBooks, SIGNAL(s_uploadImage(int)), this,
          SLOT(updateProviderImage(int)));
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
  connect(m_tabPrints, SIGNAL(sendStatusBarMessage(const QString &)), this,
          SLOT(prepareMessanger(const QString &)));
  // Artikel Bestandsänderung an Providers senden
  connect(m_tabPrints, SIGNAL(s_articleCount(int, int)), this,
          SLOT(updateArticleCount(int, int)));
  connect(m_tabPrints, SIGNAL(s_uploadImage(int)), this,
          SLOT(updateProviderImage(int)));
  int i = insertTab(index, m_tabPrints, tr("Everything else"));
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
  connect(m_tabCustomers, SIGNAL(sendStatusBarMessage(const QString &)), this,
          SLOT(prepareMessanger(const QString &)));
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
  connect(m_tabOrders, SIGNAL(sendStatusBarMessage(const QString &)), this,
          SLOT(prepareMessanger(const QString &)));
  // Artikel Bestandsänderung an Providers senden
  connect(m_tabOrders, SIGNAL(s_articleCount(int, int)), this,
          SLOT(updateArticleCount(int, int)));
  // Kunden Aufrufen
  connect(m_tabOrders, SIGNAL(s_viewCustomer(int)), this,
          SLOT(editCustomerEntry(int)));
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
  connect(m_tabProviders, SIGNAL(sendStatusBarMessage(const QString &)), this,
          SLOT(prepareMessanger(const QString &)));
  // Kunden Aufrufen
  connect(m_tabProviders, SIGNAL(s_viewCustomer(int)), this,
          SLOT(editCustomerEntry(int)));
  // Auftrag/Bestellung erstellen
  connect(m_tabProviders, SIGNAL(createOrder(const ProviderOrder &)), this,
          SLOT(createOrder(const ProviderOrder &)));
  // Einen Auftrag mit der Auftragsnummer anzeigen
  connect(m_tabProviders, SIGNAL(s_viewOrder(int)), this,
          SLOT(openOrderByOrderId(int)));
  // Einen Artikel (Buch oder Drucke) öffnen
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
  connect(m_tabViews, SIGNAL(sendStatusBarMessage(const QString &)), this,
          SLOT(prepareMessanger(const QString &)));
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
    emit sendPostMessage(Antiqua::ErrorStatus::NOTICE,
                         tr("Books tab isn't open!"));
  }
}

void Workspace::editPrintsEntry(int articleId) {
  if (articleId > 0 && indexOf(m_tabPrints) >= 0) {
    m_tabPrints->editPrintsEntry(articleId);
    setCurrentWidget(m_tabPrints);
  } else {
    emit sendPostMessage(Antiqua::ErrorStatus::NOTICE,
                         tr("Everything else tab isn't open!"));
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
    emit sendPostMessage(Antiqua::ErrorStatus::NOTICE,
                         tr("Order tab isn't open!"));
  }
}

void Workspace::createOrder(int customerId) {
  if (customerId > 0 && indexOf(m_tabOrders) >= 0) {
    m_tabOrders->createOrder(customerId);
    setCurrentWidget(m_tabOrders);
  } else {
    emit sendPostMessage(Antiqua::ErrorStatus::NOTICE,
                         tr("Order tab isn't open!"));
  }
}

void Workspace::createOrder(const ProviderOrder &order) {
  if (indexOf(m_tabOrders) >= 0) {
    m_tabOrders->createOrder(order);
    setCurrentWidget(m_tabOrders);
  } else {
    emit sendPostMessage(Antiqua::ErrorStatus::NOTICE,
                         tr("Order tab isn't open!"));
  }
}

void Workspace::addArticleOrder(int articleId) {
  if (articleId > 0 && indexOf(m_tabOrders) >= 0) {
    bool b = m_tabOrders->addArticleToOrder(articleId);
    if (b) {
      setCurrentWidget(m_tabOrders);
    }
  } else {
    emit sendPostMessage(Antiqua::ErrorStatus::NOTICE,
                         tr("Order tab isn't open!"));
  }
}

void Workspace::openOrderByOrderId(int orderId) {
  if (orderId > 0 && indexOf(m_tabOrders) >= 0) {
    if (m_tabOrders->viewOrderById(orderId))
      setCurrentWidget(m_tabOrders);
  } else {
    emit sendPostMessage(Antiqua::ErrorStatus::NOTICE,
                         tr("Order tab isn't open!"));
  }
}

void Workspace::updateArticleCount(int articleId, int count) {
  if (indexOf(m_tabProviders) >= 0) {
    // qDebug() << Q_FUNC_INFO << articleId << count;
    if (m_tabProviders->updateArticleCount(articleId, count)) {
      emit sendPostMessage(Antiqua::ErrorStatus::NOTICE, tr("successfully"));
    } else {
      emit sendPostMessage(Antiqua::ErrorStatus::NOTICE,
                           tr("an error occurred"));
    }
  } else {
    emit sendPostMessage(Antiqua::ErrorStatus::NOTICE,
                         tr("Provider tab isn't open!"));
  }
}

void Workspace::updateProviderImage(int articleId) {
  if (indexOf(m_tabProviders) >= 0) {
    if (m_tabProviders->updateProviderImage(articleId)) {
      emit sendPostMessage(Antiqua::ErrorStatus::NOTICE, tr("successfully"));
    } else {
      emit sendPostMessage(Antiqua::ErrorStatus::NOTICE,
                           tr("an error occurred"));
    }
  } else {
    emit sendPostMessage(Antiqua::ErrorStatus::NOTICE,
                         tr("Provider tab isn't open!"));
  }
}

void Workspace::closeTabClicked(int index) {
  Inventory *tab = qobject_cast<Inventory *>(widget(index));
  if (tab != nullptr && tab->isClosable()) {
    if (!tab->isWindowModified()) {
      removeTab(index);
      return;
    }
    emit sendPostMessage(Antiqua::ErrorStatus::NOTICE,
                         tr("Cant close this tab, unsafed changes!"));
  }
}

void Workspace::tabViewClicked(int i) {
  reinterpret_cast<Inventory *>(widget(i))->onEnterChanged();
}

void Workspace::prepareMessanger(const QString &msg) {
  emit sendPostMessage(Antiqua::ErrorStatus::NOTICE, msg);
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
      m_tabOrders->refreshView();
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
