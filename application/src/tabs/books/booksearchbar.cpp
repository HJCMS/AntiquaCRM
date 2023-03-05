// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booksearchbar.h"
#include "booksearchline.h"
#include "bookselectfilter.h"

#include <QIcon>
#include <QToolButton>

BookSearchBar::BookSearchBar(QWidget *parent) : TabSearchBar{parent} {
  setObjectName("book_search_bar");

  m_selectFilter = new BookSelectFilter(this);
  QString filterTip = tr("Press CTRL+Shift+F, to quickly open this Menu.");
  m_selectFilter->setToolTip(filterTip);
  addWidget(m_selectFilter);

  m_searchLeft = new BookSearchLine(this);
  addWidget(m_searchLeft);

  QToolButton *m_icontb = new QToolButton(this);
  m_icontb->setEnabled(false);
  m_icontb->setIcon(QIcon(":icons/user_group.png"));
  addWidget(m_icontb);

  m_searchRight = new BookSearchLine(this);
  addWidget(m_searchRight);

  addWidget(searchConfines());
  addSeparator();
  addWidget(stockCheckBox());

  m_searchBtn = startSearchButton();
  addWidget(m_searchBtn);

  connect(m_selectFilter, SIGNAL(currentIndexChanged(int)),
          SLOT(setFilter(int)));
  connect(m_searchLeft, SIGNAL(returnPressed()), SLOT(setSearch()));
  connect(m_searchRight, SIGNAL(returnPressed()), SLOT(setSearch()));
  connect(m_searchBtn, SIGNAL(clicked()), SLOT(setSearch()));
}

const QString BookSearchBar::getTitleSearch(const QStringList &fields) {
  QString query;
  QString inputLeft = m_searchLeft->getSearch();
  QString inputRight = m_searchRight->getSearch();
  // Standard Suchfeld
  if (m_searchLeft->isValid(getMinLength())) {
    QStringList bufferLeft;
    foreach (QString f, fields) {
      if (f != "ib_author") {
        QString fset = prepareFieldSearch(f, inputLeft);
        if (fset.isEmpty())
          continue;

        bufferLeft << fset;
      }
    }
    if (bufferLeft.count() > 0) {
      query.append("(");
      query.append(bufferLeft.join(" OR "));
      query.append(")");
    }
    bufferLeft.clear();
  }
  // Autoren Suchfeld
  if (m_searchRight->isEnabled() && m_searchRight->isValid(getMinLength())) {
    QStringList bufferRight;
    if (m_searchRight->placeholderText().contains(tr("Keyword")))
      bufferRight << prepareFieldSearch("ib_keyword", inputRight);
    else
      bufferRight << prepareFieldSearch("ib_author", inputRight);

    if (query.isEmpty())
      query.append("(");
    else
      query.append(" AND (");

    query.append(bufferRight.join(" OR "));
    query.append(")");
    bufferRight.clear();
  }
  if (query.length() < 1) {
    // NOTE prevent empty where clauses
    qWarning("INVALID_TITLE_SEARCH");
    if (!inputLeft.isEmpty())
      return QString("ib_title='" + inputLeft + "'");
    else if (!inputRight.isEmpty())
      return QString("ib_title='" + inputRight + "'");
    else
      return QString("ib_title='INVALID_TITLE_SEARCH'");
  }
  return query;
}

void BookSearchBar::setSearch() {
  if (m_searchLeft->isEnabled() && m_searchLeft->isValid(getMinLength())) {
    emit sendSearchClicked();
  } else if (m_searchRight->isEnabled() &&
             m_searchRight->isValid(getMinLength())) {
    emit sendSearchClicked();
  } else
    emit sendNotify(tr("Your input is too short, increase your search!"));
}

void BookSearchBar::setFilter(int index) {
  QVariant val = m_selectFilter->itemData(index, Qt::UserRole);
  BookSelectFilter::Filter f = qvariant_cast<BookSelectFilter::Filter>(val);
  m_searchLeft->setEnabled(true);
  m_searchLeft->clear();
  m_searchRight->clear();

  QString leftTr = tr("Search Booktitle");
  QString rightTr = tr("Authors search");

  switch (f) {
  case (BookSelectFilter::ArticleId): {
    m_searchLeft->setValidation(SearchLineEdit::Article);
    m_searchLeft->setPlaceholderText(
        tr("Single Article number or multiple separated by comma."));
    m_searchRight->setEnabled(false);
    m_searchRight->setPlaceholderText(rightTr);
    break;
  }

  case (BookSelectFilter::ISBN): {
    m_searchLeft->setValidation(SearchLineEdit::Numeric);
    m_searchLeft->setPlaceholderText(tr("Search by ISBN number"));
    m_searchRight->setEnabled(false);
    m_searchRight->setPlaceholderText(rightTr);
    break;
  }

  case (BookSelectFilter::BooksAuthor): {
    m_searchLeft->setValidation(SearchLineEdit::Strings);
    m_searchLeft->setPlaceholderText(leftTr);
    m_searchRight->setValidation(SearchLineEdit::Strings);
    m_searchRight->setEnabled(true);
    m_searchRight->setPlaceholderText(tr("and Authors"));
    break;
  }

  case (BookSelectFilter::BooksKeyword): {
    m_searchLeft->setValidation(SearchLineEdit::Strings);
    m_searchLeft->setPlaceholderText(leftTr);
    m_searchRight->setValidation(SearchLineEdit::Strings);
    m_searchRight->setEnabled(true);
    m_searchRight->setPlaceholderText(tr("Keyword"));
    break;
  }

  case (BookSelectFilter::Authors): {
    m_searchLeft->setValidation(SearchLineEdit::Strings);
    m_searchLeft->setEnabled(false);
    m_searchLeft->setPlaceholderText(leftTr);
    m_searchRight->setValidation(SearchLineEdit::Strings);
    m_searchRight->setEnabled(true);
    m_searchRight->setPlaceholderText(rightTr);
    break;
  }

  default: {
    m_searchLeft->setValidation(SearchLineEdit::Strings);
    m_searchRight->setEnabled(false);
    m_searchLeft->setPlaceholderText(leftTr);
    m_searchRight->setPlaceholderText(rightTr);
    break;
  }
  };

  emit sendFilterChanged(index);
}

void BookSearchBar::setFilterFocus() {
  m_selectFilter->setFocus();
  m_selectFilter->showPopup();
}

void BookSearchBar::setClearAndFocus() {
  m_searchLeft->clear();
  m_searchRight->clear();
  m_searchLeft->setFocus();
}

void BookSearchBar::setSearchFocus() { setClearAndFocus(); }

int BookSearchBar::searchLength() {
  return (m_searchLeft->getLength() + m_searchRight->getLength());
}

const QString BookSearchBar::getSearchStatement() {
  QJsonObject js = m_selectFilter->getFilter(m_selectFilter->currentIndex());
  QStringList fields = js.value("fields").toString().split(",");
  QString sql(withStock() ? "ib_count>0 AND " : "");
  QString operation = js.value("search").toString();
  if (operation.isEmpty())
    return QString();

  // Title und Autorensuche
  if (operation == "title_and_author") {
    sql.append("(" + getTitleSearch(fields) + ")");
    return sql;
  }

  // Buch Titlesuche
  if (operation == "title") {
    sql.append("(" + getTitleSearch(fields) + ")");
    return sql;
  }

  // Buch Autorensuche
  if (operation == "author") {
    sql.append("(" + getTitleSearch(fields) + ")");
    return sql;
  }

  // Artikel Nummersuche (107368,115110)
  if (operation == "articleId") {
    QString s = m_searchLeft->getSearch();
    s.replace(QRegExp("\\,\\s?$"), "");
    return "ib_id IN (" + s + ")";
  }

  // ISBN Suche
  if (operation == "isbn") {
    QString s = m_searchLeft->getSearch();
    s.replace(QRegExp("\\D+"), "");
    if (s.length() == 10 || s.length() == 13) {
      sql.append("ib_isbn=" + s);
      return sql;
    }
  }

  // Publisher
  if (operation == "publisher") {
    QString s = m_searchLeft->getSearch();
    sql.append("(" + prepareFieldSearch("ib_publisher", s) + ")");
    return sql;
  }

  // Lager & Stichwortsuche
  if (operation == "storage") {
    QString s = m_searchLeft->getSearch();
    s.replace(jokerPattern, "%");
    QStringList buffer;
    buffer << prepareFieldSearch("sl_storage", s);
    buffer << prepareFieldSearch("sl_identifier", s);
    buffer << prepareFieldSearch("ib_keyword", s);
    sql.append("(" + buffer.join(" OR ") + ")");
    return sql;
  }

  qWarning("Not Defined Search (%s)",
           qPrintable(js.value("search").toString()));

  return QString();
}
