// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquatabwidget.h"
#include "tabbooks.h"

/** TESTINGS */
#include <AntiquaWidgets>
#include <AntiquaCRM>
#include <QVBoxLayout>

AntiquaTabWidget::AntiquaTabWidget(QMainWindow *parent) : QTabWidget{parent} {
  setObjectName("tab_widgets_main");

  connect(tabBar(), SIGNAL(currentChanged(int)), SLOT(tabChanged(int)));
}

void AntiquaTabWidget::tabChanged(int index) {
  Inventory *m_tab = qobject_cast<Inventory *>(widget(index));
  if (m_tab != nullptr)
    m_tab->onEnterChanged();
}

bool AntiquaTabWidget::loadDefaultTabs() {
  m_books = new TabBooks(this);
  QIcon icon = m_books->windowIcon();
  insertTab(0, m_books, icon, m_books->windowTitle());

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
  // Länder
  SelectEuCountry *m_countries = new SelectEuCountry(this);
  m_countries->setInfo(tr("Countries"));
  testLayout->addWidget(m_countries);

  testLayout->addStretch(1);
  m_testing->setLayout(testLayout);
  insertTab(1, m_testing, icon, "Test Widgets");

  return true;
}
