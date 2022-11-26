// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquatabwidget.h"
#include "antiquasocketserver.h"
#include "antiquatabbar.h"

AntiquaTabWidget::AntiquaTabWidget(QMainWindow *parent) : QTabWidget{parent} {
  setObjectName("window_tabwidget");

  AntiquaCRM::ASettings cfg(this);
  cfg.setObjectName("tabwidget_settings");

  bool mouseWheel = cfg.value("mouse_wheel_actions", false).toBool();
  m_tabBar = new AntiquaTabBar(this, mouseWheel);
  setTabBar(m_tabBar);

  connect(m_tabBar, SIGNAL(sendTabChanged(int)), SLOT(setTabChanged(int)));
  connect(m_tabBar, SIGNAL(sendCloseTab(int)), SLOT(setTabToClose(int)));
  connect(m_tabBar, SIGNAL(tabBarClicked(int)), SLOT(setTabToVisit(int)));
  connect(this, SIGNAL(tabCloseRequested(int)), SLOT(setTabToClose(int)));

  createSocketListener();
}

bool AntiquaTabWidget::createSocketListener() {
  m_socket = new AntiquaSocketServer(this);
  connect(m_socket, SIGNAL(sendWindowOperation(const QJsonObject &)),
          SLOT(setAction(const QJsonObject &)));
  connect(m_socket, SIGNAL(sendPluginOperation(const QJsonObject &)),
          SIGNAL(sendPluginOperation(const QJsonObject &)));
  connect(m_socket, SIGNAL(sendStatusMessage(const QString &)),
          SIGNAL(sendStatusMessage(const QString &)));

  return m_socket->listen(AntiquaCRM::antiquaSocketName());
}

Inventory *AntiquaTabWidget::tabWidget(int index) const {
  return qobject_cast<Inventory *>(widget(index));
}

int AntiquaTabWidget::indexByName(const QString &name) const {
  for (int i = 0; i < count(); i++) {
    Inventory *m_tab = tabWidget(i);
    if (m_tab != nullptr && m_tab->tabIndexId() == name) {
      return i;
    }
  }
  return -1;
}

bool AntiquaTabWidget::addInventoryTab(const QString &name) {
  QString iName = name.trimmed().toLower();
  iName.append("_tab");
  // Check
  int index = indexByName(iName);
  if (index >= 0) {
    setCurrentIndex(index);
    return true;
  }

  // Books
  if (iName == "books_tab") {
    TabBooks *m_tab = new TabBooks(this);
    int i = addTab(m_tab, m_tab->windowIcon(), m_tab->windowTitle());
    m_tabBar->setTabCloseable(i, m_tab->isClosable());
    m_tabBar->setTabWhatsThis(i, tr("View and edit Books"));
    return true;
  }

  // Customers
  if (iName == "customers_tab") {
    TabCustomers *m_tab = new TabCustomers(this);
    int i = addTab(m_tab, m_tab->windowIcon(), m_tab->windowTitle());
    m_tabBar->setTabCloseable(i, m_tab->isClosable());
    m_tabBar->setTabWhatsThis(i, tr("View and edit Customers"));
    return true;
  }

  // Providers
  if (iName == "providers_tab") {
    TabProviders *m_tab = new TabProviders(this);
    int i = addTab(m_tab, m_tab->windowIcon(), m_tab->windowTitle());
    m_tabBar->setTabCloseable(i, m_tab->isClosable());
    m_tabBar->setTabWhatsThis(i, tr("View and import Provider orders"));
    return true;
  }

  // Orders
  if (iName == "orders_tab") {
    TabOrders *m_tab = new TabOrders(this);
    int i = addTab(m_tab, m_tab->windowIcon(), m_tab->windowTitle());
    m_tabBar->setTabCloseable(i, m_tab->isClosable());
    m_tabBar->setTabWhatsThis(i, tr("View and edit System orders"));
    return true;
  }

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << "Not in set" << iName;
#endif

  // Not in set
  return false;
}

void AntiquaTabWidget::setAction(const QJsonObject &obj) {
  if (obj.contains("window_operation")) {
    // Must be identical to the "index" call of the tab class!
    // grep -hoe '\<[a-z]\+_tab\>' tabs/*/tab*.cpp
    QString tab = obj.value("tab").toString();
    int index = indexByName(tab);
    if (index < 0) {
      qWarning("Invalid Operation call for tab actions!");
#ifdef ANTIQUA_DEVELOPEMENT
      qDebug() << Q_FUNC_INFO << "Invalid:" << obj;
#endif
      return;
    }

    Inventory *m_tab = tabWidget(index);
    if (m_tab != nullptr && m_tab->customAction(obj)) {
      setCurrentIndex(index);
    } else {
#ifdef ANTIQUA_DEVELOPEMENT
      qDebug() << Q_FUNC_INFO << "Rejected:" << obj;
#endif
      emit sendStatusMessage(tr("Request rejected!"));
    }
    return;
  }
}

void AntiquaTabWidget::addViewsTab(const QString &query) {
  int index = indexByName("views_tab");
  if (index >= 0) {
    m_views->createSearchQuery(query);
    setCurrentIndex(index);
    return;
  }

  m_views = new TabViews(this);
  m_views->createSearchQuery(query);
  int i = addTab(m_views, m_views->windowIcon(), m_views->windowTitle());
  m_tabBar->setTabCloseable(i, m_views->isClosable());
  setCurrentIndex(i);
}

void AntiquaTabWidget::setTabChanged(int index) {
  Inventory *m_tab = tabWidget(index);
  if (m_tab != nullptr)
    m_tab->onEnterChanged();
}

void AntiquaTabWidget::setTabToClose(int index) {
  Inventory *m_tab = tabWidget(index);
  if (m_tab != nullptr && m_tab->isClosable()) {
    if (!m_tab->isWindowModified()) {
      removeTab(index);
      return;
    }
    qDebug() << "Can't close this tab, unsaved changes!";
  }
}

void AntiquaTabWidget::setTabToVisit(int index) {
  Inventory *m_tab = tabWidget(index);
  if (m_tab != nullptr)
    m_tab->onEnterChanged();
}

void AntiquaTabWidget::setShowTab(const QString &tabId) {
  addInventoryTab(tabId);
}

const QMap<QString, QString> AntiquaTabWidget::availableTabs() {
  QMap<QString, QString> m;
  m.insert("books", tr("Books"));
  m.insert("customers", tr("Customers"));
  m.insert("orders", tr("Orders"));
  m.insert("providers", tr("Providers"));
  return m;
}

AntiquaTabWidget::~AntiquaTabWidget() {
  // disable socket
  if (m_socket != nullptr) {
    m_socket->close();
    m_socket->deleteLater();
  }
  // unload tabs
  for (int t = 0; t < count(); t++) {
    Inventory *m_tab = tabWidget(t);
    removeTab(t);
    m_tab->deleteLater();
  }
}
