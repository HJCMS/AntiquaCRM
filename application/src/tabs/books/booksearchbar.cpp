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
  // Standard Suchfeld
  if (m_searchLeft->getLength() >= getMinLength()) {
    QStringList bufferLeft;
    foreach (QString f, fields) {
      if (f != "ib_author") {
        QString fset = prepareFieldSet(f, p_currentLeft);
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
  if (m_searchRight->isEnabled() &&
      m_searchRight->getLength() >= getMinLength()) {
    QStringList bufferRight;
    if (m_searchRight->placeholderText().contains(tr("Keyword")))
      bufferRight << prepareFieldSet("ib_keyword", p_currentRight);
    else
      bufferRight << prepareFieldSet("ib_author", p_currentRight);

    if (query.isEmpty())
      query.append("(");
    else
      query.append(" AND (");

    query.append(bufferRight.join(" OR "));
    query.append(")");
    bufferRight.clear();
  }
  return query;
}

void BookSearchBar::setSearch() {
  if (m_searchLeft->isEnabled() && m_searchLeft->getLength() > getMinLength()) {
    QString left = m_searchLeft->getSearch();
    left = left.replace(quotePattern, "");
    left = left.replace(trimPattern, " ");
    p_currentLeft = left.trimmed();
  } else {
    p_currentLeft = QString();
    emit sendNotify(tr("Your input is too short, increase your search!"));
  }

  if (m_searchRight->isEnabled() &&
      m_searchRight->getLength() > getMinLength()) {
    QString right = m_searchRight->getSearch();
    right = right.replace(quotePattern, "");
    right = right.replace(trimPattern, " ");
    p_currentRight = right.trimmed();
  } else {
    p_currentRight = QString();
  }
  emit sendSearchClicked();
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
  QString stock = (SearchWithStock ? " AND ib_count>0" : "");

  // Title und Autorensuche
  if (js.value("search").toString() == "title_and_author") {
    return getTitleSearch(fields) + stock;
  }

  // Buch Titlesuche
  if (js.value("search").toString() == "title") {
    return getTitleSearch(fields) + stock;
  }

  // Buch Autorensuche
  if (js.value("search").toString() == "author") {
    return getTitleSearch(fields) + stock;
  }

  // Artikel Nummersuche (107368,115110)
  if (js.value("search").toString() == "articleId") {
    QString s = m_searchLeft->getSearch();
    s.replace(QRegExp("\\,\\s?$"), "");
    return "ib_id IN (" + s + ")" + stock;
  }

  // ISBN Suche
  if (js.value("search").toString() == "isbn") {
    QString s = m_searchLeft->getSearch();
    s.replace(QRegExp("\\D+"), "");
    if (s.length() == 10 || s.length() == 13)
      return "ib_isbn=" + s + stock;
  }

  // Publisher
  if (js.value("search").toString() == "publisher") {
    QString s = m_searchLeft->getSearch();
    s.replace(jokerPattern, "%");
    return "ib_publisher ILIKE '" + s + "%'" + stock;
  }

  // Lager & Stichwortsuche
  if (js.value("search").toString() == "storage") {
    QString s = m_searchLeft->getSearch();
    s.replace(jokerPattern, "%");
    QString sql;
    sql = ("ib_count>0 AND (sl_storage ILIKE '");
    sql.append(s + "' OR sl_identifier ILIKE '" + s + "%')");
    sql.append(" OR (ib_keyword ILIKE '" + s + "%')");
    return sql + stock;
  }

  qWarning("Not Defined Search (%s)",
           qPrintable(js.value("search").toString()));

  return QString();
}

bool BookSearchBar::withStock() { return SearchWithStock; }
