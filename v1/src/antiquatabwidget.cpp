// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquatabwidget.h"
#include "tabbooks.h"

/** TESTINGS */
#include "bookbinding.h"
#include "postalcodeedit.h"
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
  testLayout->addWidget(m_binding);
  // Postleitzahlen
  PostalCodeEdit *m_plz = new PostalCodeEdit(m_testing);
  testLayout->addWidget(m_plz);
  // Kunden Tabelle
  AntiquaCRM::ASqlTable *m_kunde = new AntiquaCRM::ASqlTable("customers");
  m_kunde->setValue("c_since", QDateTime::currentDateTime());
  m_kunde->setValue("c_phone_0", QString());
  qDebug() << m_kunde->getType("c_since").name();

  m_testing->setLayout(testLayout);
  insertTab(1, m_testing, icon, "Test Widgets");

  return true;
}
