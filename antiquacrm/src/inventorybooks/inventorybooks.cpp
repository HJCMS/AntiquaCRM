// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventorybooks.h"
#include "applsettings.h"
#include "bookeditor.h"
#include "bookstableview.h"
#include "editordialog.h"
#include "statsbookbar.h"
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
static const QList<SearchFilter> bookSearchFilter() {
  SearchFilter a;
  QList<SearchFilter> filter;
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
  return filter;
}

InventoryBooks::InventoryBooks(int index, QTabWidget *parent)
    : Inventory{index, parent} {
  setObjectName("InventoryBooks");
  setWindowTitle("TabBooks");

  ApplSettings cfg;
  minLength = cfg.value("search/minlength", 5).toInt();

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("InventoryBooksLayout");

  /**
    @brief m_searchBar
    Siehe auch @ref updateValidator(int)!
  */
  m_searchBar = new SearchBar(this);
  m_searchBar->setValidation(SearchBar::Pattern);
  m_searchBar->addSearchFilters(bookSearchFilter());
  layout->addWidget(m_searchBar);

  m_tableView = new BooksTableView(this);
  layout->addWidget(m_tableView);

  m_statsBookBar = new StatsBookBar(this);
  layout->addWidget(m_statsBookBar);

  setLayout(layout);

  // Such anfragen
  connect(m_searchBar, SIGNAL(searchTextChanged(const QString &)), this,
          SLOT(parseLineInput(const QString &)));

  connect(m_searchBar, SIGNAL(searchClicked()), this, SLOT(searchConvert()));

  connect(m_searchBar, SIGNAL(currentFilterChanged(int)), this,
          SLOT(updateValidator(int)));

  // Verlaufs abfragen
  connect(m_statsBookBar, SIGNAL(s_queryHistory(const QString &)), m_tableView,
          SLOT(queryHistory(const QString &)));

  connect(m_statsBookBar, SIGNAL(s_createEntryClicked()), this,
          SLOT(createBookArticle()));

  // ID wurde in Tabellenansicht ausgewählt
  connect(m_tableView, SIGNAL(s_articleSelected(int)), this,
          SLOT(articleSelected(int)));
}

void InventoryBooks::parseLineInput(const QString &str) {
  if (str.length() <= minLength)
    return;

  searchConvert();
}

void InventoryBooks::searchConvert() {
  if (m_searchBar->currentSearchText().length() < 2)
    return;

  QString buf = m_searchBar->currentSearchText();
  QRegExp reg("[\\'\\\"]+");
  buf.replace(reg, "");

  reg.setPattern("(\\s|\\t)+");
  buf.replace(reg, " ");

  buf = buf.trimmed();

  if (buf.length() >= 2) {
    SearchStatement s;
    s.SearchField =
        m_searchBar->getSearchFilter(m_searchBar->currentFilterIndex());
    s.SearchString = buf;
    qDebug("QueryStatement:'%s':'%s'", qPrintable(s.SearchField),
           qPrintable(s.SearchString));
    m_tableView->queryStatement(s);
  }
}

void InventoryBooks::openEditor(const QString &sql) {
  // qDebug() << "InventoryBooks::openEditor" << sql;
  EditorDialog *dialog = new EditorDialog(this);
  dialog->setWindowTitle(tr("Edit Book"));
  dialog->setMinimumSize(850,650);
  dialog->setShortcutEnabled(false);

  m_bookEditor = new BookEditor(dialog);
  if (!sql.isEmpty()) {
    m_bookEditor->readDataBaseEntry(sql);
  }
  dialog->setMainWidget(m_bookEditor);
  connect(dialog, SIGNAL(s_restoreDataset()), m_bookEditor,
          SLOT(restoreDataset()));

  connect(dialog, SIGNAL(s_commitDataset()), m_bookEditor, SLOT(saveData()));

  dialog->show();
}

void InventoryBooks::createBookArticle() { openEditor(QString()); }

void InventoryBooks::articleSelected(int id) {
  if (id < 1)
    return;

  QString s("ib_id=");
  s.append(QString::number(id));
  openEditor(s);
}

void InventoryBooks::updateValidator(int id) {
  switch (id) {
  case 2:
    m_searchBar->setValidation(SearchBar::Number);
    break;

  case 3:
    m_searchBar->setValidation(SearchBar::Number);
    break;

  default:
    m_searchBar->setValidation(SearchBar::Pattern);
    break;
  };
}
