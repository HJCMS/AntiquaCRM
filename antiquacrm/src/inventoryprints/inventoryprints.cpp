// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventoryprints.h"
#include "applsettings.h"
#include "printseditor.h"
#include "printstableview.h"
#include "searchbar.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>
#include <QtWidgets/QVBoxLayout>

/**
   @brief bookSearchFilter
   Muss für jedes Suchtab einzeln definiert sein.
   @ref SearchBar::addSearchFilters
   @return const QList<SearchFilter>
*/
static const QList<SearchFilter> bookSearchFilter() {
  SearchFilter a;
  QList<SearchFilter> filter;
  a.index = 0;
  a.title = QObject::tr("Title");
  a.filter = QString("");
  filter.append(a);
  a.index = 1;
  a.title = QObject::tr("Title (starts with)");
  a.filter = QString("title_first");
  filter.append(a);
  a.index = 2;
  a.title = QObject::tr("Article ID");
  a.filter = QString("id");
  filter.append(a);
  a.index = 3;
  a.title = QObject::tr("Author");
  a.filter = QString("author");
  filter.append(a);
  return filter;
}

InventoryPrints::InventoryPrints(int index, QTabWidget *parent)
    : QWidget{parent} {
  setObjectName("InventoryPrints");
  setWindowTitle("TabPrints");

  ApplSettings cfg;
  minLength = cfg.value("search/startlength", 5).toInt();

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("InventoryPrintsLayout");

  m_stackedWidget = new QStackedWidget(this);
  m_stackedWidget->setObjectName("prints_stacked_widget");

  //Begin m_stackedWidget(0)
  QWidget *siteOneWidget = new QWidget(m_stackedWidget);
  siteOneWidget->setObjectName("prints_site_one_widget");
  QVBoxLayout *siteOneLayout = new QVBoxLayout(siteOneWidget);
  siteOneLayout->setObjectName("prints_site_one_layout");

  m_searchBar = new SearchBar(this);
  m_searchBar->setValidation(SearchBar::Pattern);
  m_searchBar->addSearchFilters(bookSearchFilter());
  siteOneLayout->addWidget(m_searchBar);

  m_tableView = new PrintsTableView(this);
  siteOneLayout->addWidget(m_tableView);
  siteOneWidget->setLayout(siteOneLayout);
  m_stackedWidget->insertWidget(0, siteOneWidget);
  //END m_stackedWidget(0)

  //Begin m_stackedWidget(1)
  m_printsEditor = new PrintsEditor(m_stackedWidget);
  m_stackedWidget->insertWidget(1, m_printsEditor);
  //END m_stackedWidget(1)

  layout->addWidget(m_stackedWidget);

  setLayout(layout);
}
