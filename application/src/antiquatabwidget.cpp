// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquatabwidget.h"
#include "antiquatabbar.h"

#include <AntiquaCRM>

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
  m_server = new AntiquaCRM::AReceiver(this);
  connect(m_server, SIGNAL(sendWindowOperation(const QJsonObject &)),
          SLOT(setAction(const QJsonObject &)));
  connect(m_server, SIGNAL(sendPluginOperation(const QJsonObject &)),
          SIGNAL(sendPluginOperation(const QJsonObject &)));
  connect(m_server, SIGNAL(sendInfoMessage(const QString &)),
          SIGNAL(sendInfoMessage(const QString &)));
  connect(m_server, SIGNAL(sendWarnMessage(const QString &)),
          SIGNAL(sendWarnMessage(const QString &)));

  return m_server->listen(AntiquaCRM::AUtil::socketName());
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

  // Prints and Stitches
  if (iName == "printsstitches_tab") {
    TabPrintsStitches *m_tab = new TabPrintsStitches(this);
    int i = addTab(m_tab, m_tab->windowIcon(), m_tab->windowTitle());
    m_tabBar->setTabCloseable(i, m_tab->isClosable());
    m_tabBar->setTabWhatsThis(i, tr("View Print and Stitches"));
    return true;
  }

  // Cd's Vinyl
  if (iName == "cdvinyl_tab") {
    TabCDVinyl *m_tab = new TabCDVinyl(this);
    int i = addTab(m_tab, m_tab->windowIcon(), m_tab->windowTitle());
    m_tabBar->setTabCloseable(i, m_tab->isClosable());
    m_tabBar->setTabWhatsThis(i, tr("View Cd and Vinyl"));
    return true;
  }

  // Various
  if (iName == "various_tab") {
    TabVarious *m_tab = new TabVarious(this);
    int i = addTab(m_tab, m_tab->windowIcon(), m_tab->windowTitle());
    m_tabBar->setTabCloseable(i, m_tab->isClosable());
    m_tabBar->setTabWhatsThis(i, tr("Various"));
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
    // grep -hoe '\<Inventory\W*\<[a-z]\+_tab\>' tabs/*/tab*.cpp
    QString tab = obj.value("tab").toString();
    int index = indexByName(tab);
    if (index < 0) {
      // Wenn nicht angezeigt - dann öffnen!
      addInventoryTab(tab.replace("_tab", ""));
    }

    Inventory *m_tab = tabWidget(index);
    if (m_tab != nullptr && m_tab->customAction(obj)) {
      setCurrentIndex(index);
    } else {
#ifdef ANTIQUA_DEVELOPEMENT
      qDebug() << Q_FUNC_INFO << "Rejected:" << obj;
#endif
      emit sendWarnMessage(tr("Can not open '%1'!").arg(tab));
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

void AntiquaTabWidget::addStatisticsTab(const QString &query) {
  int index = indexByName("tab_statistics");
  if (index >= 0) {
    m_statistics->createSearchQuery(query);
    setCurrentIndex(index);
    return;
  }

  // Statistics
  m_statistics = new TabStatistics(this);
  m_statistics->createSearchQuery(query);
  int i = addTab(m_statistics, m_statistics->windowIcon(),
                 m_statistics->windowTitle());
  m_tabBar->setTabCloseable(i, m_statistics->isClosable());
  setCurrentIndex(i);
}

void AntiquaTabWidget::addReportsTab(const QString &query) {
  int index = indexByName("reports_tab");
  if (index >= 0) {
    m_reports->createSearchQuery(query);
    setCurrentIndex(index);
    return;
  }

  m_reports = new TabReports(this);
  m_reports->createSearchQuery(query);
  int i = addTab(m_reports, m_reports->windowIcon(), m_reports->windowTitle());
  m_tabBar->setTabCloseable(i, m_reports->isClosable());
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
    QString title = m_tab->windowTitle();
    if (!m_tab->isWindowModified()) {
      removeTab(index);
      return;
    }
    emit sendWarnMessage(tr("Unsaved changes for tab '%1'!").arg(title));
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << "Can't close this tab, unsaved changes!" << title;
#endif
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
  m.insert("printsstitches", tr("Prints and Stitches"));
  m.insert("cdvinyl", tr("CD and Vinyl"));
  m.insert("various", tr("Various"));
  return m;
}

AntiquaTabWidget::~AntiquaTabWidget() {
  // disable socket
  if (m_server != nullptr) {
    m_server->close();
    m_server->deleteLater();
  }
  // unload tabs
  for (int t = 0; t < count(); t++) {
    Inventory *m_tab = tabWidget(t);
    removeTab(t);
    m_tab->deleteLater();
  }
}
