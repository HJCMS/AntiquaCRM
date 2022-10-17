// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquatabwidget.h"
#include "antiquatabbar.h"
#include "tabbooks.h"

/** TESTINGS */
#include <AntiquaCRM>
#include <QVBoxLayout>

AntiquaTabWidget::AntiquaTabWidget(QMainWindow *parent) : QTabWidget{parent} {
  setObjectName("window_tabwidget");
  setMinimumSize(800, 550);

  AntiquaCRM::ASettings cfg(this);
  cfg.setObjectName("tabwidget_settings");

  bool mouseWheel = cfg.value("window/MouseWheelActions", false).toBool();
  m_tabBar = new AntiquaTabBar(this, mouseWheel);
  setTabBar(m_tabBar);

  connect(m_tabBar, SIGNAL(sendTabChanged(int)), SLOT(setTabChanged(int)));
  connect(m_tabBar, SIGNAL(sendCloseTab(int)), SLOT(setTabToClose(int)));
  connect(m_tabBar, SIGNAL(tabBarClicked(int)), SLOT(setTabToVisit(int)));
  connect(this, SIGNAL(tabCloseRequested(int)), SLOT(setTabToClose(int)));
}

Inventory *AntiquaTabWidget::tabWidget(int index) const {
  return qobject_cast<Inventory *>(widget(index));
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
    qDebug() << tr("Cant close this tab, unsafed changes!");
  }
}

void AntiquaTabWidget::setTabToVisit(int index) {
  Inventory *m_tab = tabWidget(index);
  if (m_tab != nullptr)
    m_tab->onEnterChanged();
}

bool AntiquaTabWidget::loadDefaultTabs() {
  m_books = new TabBooks(this);
  insertTab(0, m_books, m_books->windowIcon(), m_books->windowTitle());

  // TESTS
  m_testing = new QWidget(this);
  QVBoxLayout *testLayout = new QVBoxLayout(m_testing);
  // Buchbindungen
  BookBinding *m_binding = new BookBinding(m_testing);
  m_binding->setInfo(tr("Book binding"));
  testLayout->addWidget(m_binding);
  // Postleitzahlen
  PostalCodeEdit *m_plz = new PostalCodeEdit(m_testing);
  m_plz->setInfo(tr("Postalcode"));
  testLayout->addWidget(m_plz);
  // Kunden Tabelle
  AntiquaCRM::ASqlTable *m_kunde = new AntiquaCRM::ASqlTable("customers");
  m_kunde->setValue("c_since", QDateTime::currentDateTime());
  m_kunde->setValue("c_phone_0", QString());
  qDebug() << m_kunde->getType("c_since").name();
  // Storage
  StorageLocation *m_storage = new StorageLocation(this);
  m_storage->setInfo(tr("Storage"));
  testLayout->addWidget(m_storage);
  // Date INFO
  AntiquaDateInfo *m_dateInfo = new AntiquaDateInfo(this);
  m_dateInfo->setInfo(tr("Last changed"));
  testLayout->addWidget(m_dateInfo);
  // Date ConditionEdit
  ConditionEdit *m_condition = new ConditionEdit(this);
  m_condition->setInfo(tr("Condition"));
  testLayout->addWidget(m_condition);
  // EU-Länder
  SelectEuCountry *m_countries = new SelectEuCountry(this);
  m_countries->setInfo(tr("EU Countries"));
  testLayout->addWidget(m_countries);

  testLayout->addStretch(1);
  m_testing->setLayout(testLayout);
  insertTab(1, m_testing, Inventory::getTabIcon(), "Test Widgets");

  return true;
}
