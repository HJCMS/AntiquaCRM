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

InventoryBooks::InventoryBooks(int index, QTabWidget *parent)
    : Inventory{index, parent} {
  setObjectName("InventoryBooks");
  setWindowTitle("TabBooks");

  ApplSettings cfg;
  minLength = cfg.value("search/minlength", 5).toInt();

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("InventoryBooksLayout");

  // begin SearchBar
  m_searchBar = new SearchBar(this);
  m_searchBar->setValidation(SearchBar::Pattern);

  /**
   \note Wenn hier etwas geändert wird!
    Dann muss SearchLineEdit::updatePlaceHolder(int)
    auch geändert werden!
  */
  SearchFilter a;
  QList<SearchFilter> filter;
  a.index = 0;
  a.title = tr("Book Title");
  a.filter = QString("title");
  filter.append(a);
  a.index = 1;
  a.title = tr("Article ID");
  a.filter = QString("id");
  filter.append(a);
  a.index = 2;
  a.title = tr("ISBN");
  a.filter = QString("isbn");
  filter.append(a);
  a.index = 3;
  a.title = tr("Author");
  a.filter = QString("author");
  filter.append(a);
  m_searchBar->addSearchFilters(filter);

  layout->addWidget(m_searchBar);
  // end SearchBar

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
          SLOT(updatePlaceHolder(int)));

  // Verlaufs abfragen
  connect(m_statsBookBar, SIGNAL(s_queryHistory(const QString &)), m_tableView,
          SLOT(queryHistory(const QString &)));
  // ID in Tabelle ausgewählt
  connect(m_tableView,
          SIGNAL(articleIdSelected(const QHash<QString, QString> &)), this,
          SLOT(selectArticleId(const QHash<QString, QString> &)));
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
  qDebug() << "InventoryBooks::selectArticleId" << sql;
  EditorDialog *dialog = new EditorDialog(this);
  dialog->setWindowTitle(tr("Edit Book"));

  m_bookEditor = new BookEditor(dialog);
  m_bookEditor->editDataBaseEntry(sql);

  dialog->setMainWidget(m_bookEditor);
  connect(dialog, SIGNAL(s_restoreDataset()), m_bookEditor,
          SLOT(restoreDataset()));

  dialog->show();
}

void InventoryBooks::selectArticleId(const QHash<QString, QString> &c) {
  if (c.isEmpty())
    return;

  QHashIterator<QString, QString> i(c);
  while (i.hasNext()) {
    i.next();
    QString s("ib_id=");
    s.append(i.key());
    s.append(" AND ib_title ILIKE '");
    s.append(i.value());
    s.append("'");
    openEditor(s);
    break;
  }
}

void InventoryBooks::updatePlaceHolder(int id) {
  switch (id) {
  case 0:
    m_searchBar->setValidation(SearchBar::Pattern);
    break;

  case 1:
    m_searchBar->setValidation(SearchBar::Number);
    break;

  case 2:
    m_searchBar->setValidation(SearchBar::Number);
    break;

  case 3:
    m_searchBar->setValidation(SearchBar::Pattern);
    break;

  default:
    m_searchBar->setValidation(SearchBar::Pattern);
    break;
  };
}
