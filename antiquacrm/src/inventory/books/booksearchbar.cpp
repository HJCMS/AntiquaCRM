// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booksearchbar.h"
#include "antiqua_global.h"
#include "myicontheme.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonValue>
#include <QToolButton>

SearchFilterBox::SearchFilterBox(QWidget *parent) : QComboBox{parent} {
  setSizeAdjustPolicy(QComboBox::AdjustToContents);
  QIcon icon(myIcon("search"));
  insertItem(0, icon, tr("Book Title and Author"), Filter::BookAuthor);
  setItemData(0, tr("Search in Book titles and authors"), Qt::ToolTipRole);

  insertItem(1, icon, tr("Book Title"), Filter::Books);
  setItemData(1, tr("Search in Book titles"), Qt::ToolTipRole);

  insertItem(2, icon, tr("Article Id"), Filter::ArticleId);
  setItemData(2, tr("Multiple searches separated by commas"), Qt::ToolTipRole);

  insertItem(3, icon, tr("ISBN"), Filter::ISBN);
  setItemData(3, tr("Only Numbers are allowed for input"), Qt::ToolTipRole);

  insertItem(4, icon, tr("Author"), Filter::Authors);
  setItemData(4, tr("Search for Book authors"), Qt::ToolTipRole);

  insertItem(5, icon, tr("Book Publisher"), Filter::Publisher);
  setItemData(5, tr("Search Book Publishers"), Qt::ToolTipRole);

  insertItem(6, icon, tr("Duration by Keyword"), Filter::Storage);
  setItemData(6, tr("Search by keyword and storage location"), Qt::ToolTipRole);
}

const QJsonObject SearchFilterBox::getFilter(int index) {
  int i = (index >= 0) ?: currentIndex();
  QVariant val = itemData(index, Qt::UserRole);
  SearchFilterBox::Filter f = qvariant_cast<SearchFilterBox::Filter>(val);
  QJsonObject obj;
  switch (f) {
  case (Filter::BookAuthor): {
    obj.insert("search", QJsonValue("title_and_author"));
    obj.insert("fields", QJsonValue("ib_storage,ib_title_extended,ib_author"));
    break;
  }

  case (Filter::Books): {
    obj.insert("search", QJsonValue("title"));
    obj.insert("fields", QJsonValue("ib_title,ib_title_extended"));
    break;
  }

  case (Filter::ArticleId): {
    obj.insert("search", QJsonValue("articleId"));
    obj.insert("fields", QJsonValue("ib_id"));
    break;
  }

  case (Filter::ISBN): {
    obj.insert("search", QJsonValue("isbn"));
    obj.insert("fields", QJsonValue("ib_isbn"));
    break;
  }

  case (Filter::Authors): {
    obj.insert("search", QJsonValue("author"));
    obj.insert("fields", QJsonValue("ib_author"));
    break;
  }

  case (Filter::Publisher): {
    obj.insert("search", QJsonValue("publisher"));
    obj.insert("fields", QJsonValue("ib_publisher"));
    break;
  }

  case (Filter::Storage): {
    obj.insert("search", QJsonValue("storage"));
    obj.insert("fields", QJsonValue("ib_storage"));
    break;
  }

  default:
    obj.insert("search", QJsonValue("title"));
    obj.insert("fields", QJsonValue("ib_title,ib_title_extended"));
    break;
  };

  return obj;
}

SearchLineEdit::SearchLineEdit(QWidget *parent) : QLineEdit{parent} {
  setClearButtonEnabled(true);
  setPlaceholderText(tr("Search for"));

  m_text_validator = new QRegExpValidator(strPattern, this);
  m_num_validator = new QRegExpValidator(numPattern, this);
  m_article_validator = new QRegExpValidator(articlePattern, this);
  setValidator(m_text_validator);
}

void SearchLineEdit::setValidation(SearchLineEdit::Validator v) {
  if (v == SearchLineEdit::Article)
    setValidator(m_article_validator);
  else if (v == SearchLineEdit::Numeric)
    setValidator(m_num_validator);
  else
    setValidator(m_text_validator);
}

BookSearchBar::BookSearchBar(QWidget *parent) : QToolBar{parent} {
  setObjectName("book_search_bar");
  setOrientation(Qt::Horizontal);
  setFloatable(false);
  setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

  m_filterSection = new SearchFilterBox(this);
  QString filterTip = tr("Press CTRL+F, to quickly open this Menu.");
  m_filterSection->setToolTip(filterTip);
  addWidget(m_filterSection);
  addSeparator();

  m_searchLeft = new SearchLineEdit(this);
  QString searchTip = tr("Press CTRL+S, it clears the input for a new search.");
  m_searchLeft->setToolTip(searchTip);
  m_searchLeft->setStatusTip(
      tr("You can use a wildcard * to broaden the search."));
  m_searchLeft->setPlaceholderText(tr("Search Title"));
  addWidget(m_searchLeft);
  addSeparator();

  QToolButton *m_icontb = new QToolButton(this);
  m_icontb->setEnabled(false);
  m_icontb->setIcon(myIcon("group"));
  addWidget(m_icontb);

  m_searchRight = new SearchLineEdit(this);
  m_searchRight->setPlaceholderText(tr("Search Authors"));
  addWidget(m_searchRight);
  addSeparator();

  ac_search = addAction(myIcon("search"), tr("Start search"));
  ac_search->setToolTip(tr("Start search"));

  connect(m_filterSection, SIGNAL(currentIndexChanged(int)), this,
          SLOT(filterChanged(int)));

  connect(m_searchLeft, SIGNAL(returnPressed()), this,
          SLOT(prepareSearchQuery()));

  connect(m_searchRight, SIGNAL(returnPressed()), this,
          SLOT(prepareSearchQuery()));

  connect(ac_search, SIGNAL(triggered()), this, SIGNAL(searchClicked()));
}

void BookSearchBar::filterChanged(int index) {
  QVariant val = m_filterSection->itemData(index, Qt::UserRole);
  SearchFilterBox::Filter f = qvariant_cast<SearchFilterBox::Filter>(val);
  m_searchLeft->setEnabled(true);

  switch (f) {
  case (SearchFilterBox::ArticleId): {
    m_searchLeft->setValidation(SearchLineEdit::Article);
    m_searchRight->setEnabled(false);
    break;
  }

  case (SearchFilterBox::ISBN): {
    m_searchLeft->setValidation(SearchLineEdit::Numeric);
    m_searchRight->setEnabled(false);
    break;
  }

  case (SearchFilterBox::BookAuthor): {
    m_searchLeft->setValidation(SearchLineEdit::Strings);
    m_searchRight->setValidation(SearchLineEdit::Strings);
    m_searchRight->setEnabled(true);
    break;
  }

  case (SearchFilterBox::Authors): {
    m_searchLeft->setValidation(SearchLineEdit::Strings);
    m_searchLeft->setEnabled(false);
    m_searchRight->setValidation(SearchLineEdit::Strings);
    m_searchRight->setEnabled(true);
    break;
  }

  default: {
    m_searchLeft->setValidation(SearchLineEdit::Strings);
    m_searchRight->setEnabled(false);
    break;
  }
  };
  m_searchLeft->setPlaceholderText(m_filterSection->itemText(index));
  emit currentFilterChanged(index);
}

void BookSearchBar::prepareSearchQuery() {
  QRegExp pattern("[\\'\\\"]+");
  QString left = m_searchLeft->text();
  QString right = m_searchRight->text();

  left = left.replace(pattern, "");
  right = right.replace(pattern, "");

  pattern.setPattern("[\\s\\t\\n\\r]+");
  right = left.replace(pattern, " ");
  right = right.replace(pattern, " ");

  QString buffer(left.trimmed());
  if (!right.isEmpty()) {
    buffer.append(" ");
    buffer.append(right);
  }

  p_currentSearch = buffer;

  qDebug() << Q_FUNC_INFO << p_currentSearch;
  emit searchClicked();
}

void BookSearchBar::setFocus() { m_searchLeft->setFocus(); }

void BookSearchBar::clearAndFocus() {
  m_searchLeft->clear();
  m_searchRight->clear();
  m_searchLeft->setFocus();
}

const QJsonObject BookSearchBar::getSearchFilter(int index) {
  return m_filterSection->getFilter(index);
}

SearchFilterBox::Filter BookSearchBar::currentFilterIndex() {
  return p_currentFilter;
}

const QString BookSearchBar::currentSearchText() {
  return p_currentSearch.trimmed();
}
