// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventorybooks.h"
#include "applsettings.h"
#include "bookeditor.h"
#include "bookstableview.h"
#include "editordialog.h"
#include "messagebox.h"
#include "searchbar.h"
#include "statsactionbar.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtCore/QHash>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>
#include <QtWidgets/QVBoxLayout>

/**
   @brief bookSearchFilter
   Muss für jedes Suchtab einzeln definiert sein.
   @ref SearchBar::addSearchFilters
   @return const QList<SearchFilter>
*/
static const QList<SearchBar::SearchFilter> bookSearchFilter() {
  SearchBar::SearchFilter a;
  QList<SearchBar::SearchFilter> filter;
  a.index = 0;
  a.title = QObject::tr("Book Title");
  a.filter = QString("");
  filter.append(a);
  a.index = 1;
  a.title = QObject::tr("Book Title (starts with)");
  a.filter = QString("title_first");
  filter.append(a);
  a.index = 2;
  a.title = QObject::tr("Article ID");
  a.filter = QString("id");
  filter.append(a);
  a.index = 3;
  a.title = QObject::tr("ISBN");
  a.filter = QString("isbn");
  filter.append(a);
  a.index = 4;
  a.title = QObject::tr("Author");
  a.filter = QString("author");
  filter.append(a);
  a.index = 5;
  a.title = QObject::tr("Publisher");
  a.filter = QString("publisher");
  filter.append(a);
  return filter;
}

InventoryBooks::InventoryBooks(int index, QTabWidget *parent)
    : QWidget{parent}, tabIndex{index} {
  setObjectName("InventoryBooks");
  setWindowTitle("TabBooks");

  ApplSettings cfg;
  minLength = cfg.value("search/startlength", 5).toInt();

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("inventory_books_layout");

  m_stackedWidget = new QStackedWidget(this);
  m_stackedWidget->setObjectName("books_stacked_widget");

  // BEGIN Page#0
  QWidget *siteOneWidget = new QWidget(m_stackedWidget);
  siteOneWidget->setObjectName("books_site_one_widget");
  QVBoxLayout *siteOneLayout = new QVBoxLayout(siteOneWidget);
  siteOneLayout->setObjectName("books_site_one_layout");

  /**
    @brief m_searchBar
    Siehe auch @ref updateValidator(int)!
  */
  m_searchBar = new SearchBar(this);
  m_searchBar->setValidation(SearchBar::Pattern);
  m_searchBar->addSearchFilters(bookSearchFilter());
  siteOneLayout->addWidget(m_searchBar);

  m_tableView = new BooksTableView(this);
  siteOneLayout->addWidget(m_tableView);

  m_statsBookBar = new StatsActionBar(this);
  siteOneLayout->addWidget(m_statsBookBar);
  m_stackedWidget->insertWidget(0, siteOneWidget);
  // END Page#0

  // BEGIN Page#1
  m_bookEditor = new BookEditor(m_stackedWidget);
  m_stackedWidget->insertWidget(1, m_bookEditor);
  m_bookEditor->setEnabled(false);
  // END Page#1

  m_stackedWidget->setCurrentIndex(0);
  layout->addWidget(m_stackedWidget);

  setLayout(layout);

  // Signals
  connect(m_searchBar, SIGNAL(searchTextChanged(const QString &)), this,
          SLOT(searchConvert(const QString &)));

  connect(m_searchBar, SIGNAL(searchClicked()), this, SLOT(searchConvert()));

  connect(m_searchBar, SIGNAL(currentFilterChanged(int)), this,
          SLOT(updateValidator(int)));

  connect(m_statsBookBar, SIGNAL(s_queryHistory(const QString &)), m_tableView,
          SLOT(queryHistory(const QString &)));

  connect(m_statsBookBar, SIGNAL(s_refreshView()), m_tableView,
          SLOT(refreshView()));

  connect(m_tableView, SIGNAL(s_articleSelected(int)), this,
          SLOT(articleSelected(int)));

  connect(m_tableView, SIGNAL(s_rowsChanged(int)), m_statsBookBar,
          SLOT(showRowCount(int)));

  connect(m_tableView, SIGNAL(s_newEntryPlease()), this,
          SLOT(createBookEntry()));

  connect(m_bookEditor, SIGNAL(s_sendMessage(const QString &)), this,
          SLOT(displayMessageBox(const QString &)));

  connect(m_bookEditor, SIGNAL(s_leaveEditor()), this, SLOT(openTableView()));
  connect(m_bookEditor, SIGNAL(s_isModified(bool)), this,
          SLOT(setClosable(bool)));
}

void InventoryBooks::searchConvert(const QString &query) {
  if (query.length() <= minLength)
    return;

  searchConvert();
}

void InventoryBooks::searchConvert() {
  if (m_searchBar->currentSearchText().length() < 2)
    return;

  QString buf = m_searchBar->currentSearchText();
  if (buf.length() >= 2) {
    SearchStatement s;
    s.SearchField =
        m_searchBar->getSearchFilter(m_searchBar->currentFilterIndex());
    s.SearchString = buf;
    qDebug("QueryStatement:'%s':'%s'", qPrintable(s.SearchField),
           qPrintable(s.SearchString));

    if (m_tableView != nullptr)
      m_tableView->queryStatement(s);
  }
}

void InventoryBooks::openTableView() {
  m_stackedWidget->setCurrentIndex(0);
  m_bookEditor->setEnabled(false);
}

void InventoryBooks::openEditor(const QString &condition) {
  if (!condition.isEmpty()) {
    m_bookEditor->setEnabled(true);
    m_bookEditor->editBookEntry(condition);
    m_stackedWidget->setCurrentWidget(m_bookEditor);
  }
}

void InventoryBooks::displayMessageBox(const QString &msg) {
  MessageBox *box = new MessageBox(this);
  box->notice(msg);
}

void InventoryBooks::createBookEntry() {
  m_bookEditor->createBookEntry();
  m_stackedWidget->setCurrentWidget(m_bookEditor);
}

void InventoryBooks::articleSelected(int id) {
  if (id < 1)
    return;

  QString s("ib_id=");
  s.append(QString::number(id));
  openEditor(s);
}

void InventoryBooks::updateValidator(int id) {
  switch (id) {
  case 2: /**< Artikel ID */
  case 3: /**< ISBN */
    m_searchBar->setValidation(SearchBar::Number);
    break;

  default: /**< Zeichenketten */
    m_searchBar->setValidation(SearchBar::Pattern);
    break;
  };
}
