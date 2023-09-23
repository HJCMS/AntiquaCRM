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

  p_length = getDefaultLength();

  connect(m_selectFilter, SIGNAL(currentIndexChanged(int)),
          SLOT(setFilter(int)));
  connect(m_searchLeft, SIGNAL(returnPressed()), SLOT(setSearch()));
  connect(m_searchRight, SIGNAL(returnPressed()), SLOT(setSearch()));
  connect(m_searchBtn, SIGNAL(clicked()), SLOT(setSearch()));
}

const QString BookSearchBar::getTitleSearch(const QStringList &fields) {
  QString _sql;
  const QString _left = m_searchLeft->getSearch();
  const QString _right = m_searchRight->getSearch();
  // Standard Suchfeld
  if (m_searchLeft->isValid(getMinLength())) {
    QStringList _s;
    foreach (QString f, fields) {
      if (f != "ib_author") {
        QString fset = prepareFieldSearch(f, _left);
        if (fset.isEmpty())
          continue;

        _s << fset;
      }
    }
    if (_s.count() > 0) {
      _sql.append("(");
      _sql.append(_s.join(" OR "));
      _sql.append(")");
    }
    _s.clear();
  }
  // Autoren Suchfeld
  if (m_searchRight->isEnabled() && m_searchRight->isValid(getMinLength())) {
    QStringList _s;
    if (fields.contains("ib_keyword"))
      _s << prepareFieldSearch("ib_keyword", _right);
    else
      _s << prepareFieldSearch("ib_author", _right);

    if (_sql.isEmpty())
      _sql.append("(");
    else
      _sql.append(" AND (");

    _sql.append(_s.join(" OR "));
    _sql.append(")");
    _s.clear();
  }

  // fallback
  if (_sql.length() < 1) {
    // NOTE prevent empty where clauses
    qWarning("INVALID_TITLE_SEARCH");
    if (!_left.isEmpty())
      return QString("ib_title='" + _left + "'");
    else if (!_right.isEmpty())
      return QString("ib_title='" + _right + "'");
    else
      return QString("ib_title='INVALID_TITLE_SEARCH'");
  }

  return _sql;
}

void BookSearchBar::setSearch() {
  QVariant _v = m_selectFilter->currentData(Qt::UserRole);
  if (qvariant_cast<BookSelectFilter::Filter>(_v) ==
      BookSelectFilter::ArticleId) {
    emit sendSearchClicked();
  } else if (m_searchLeft->isEnabled() &&
             m_searchLeft->isValid(getMinLength())) {
    emit sendSearchClicked();
  } else if (m_searchRight->isEnabled() &&
             m_searchRight->isValid(getMinLength())) {
    emit sendSearchClicked();
  } else {
    emit sendNotify(tr("Your input is too short, increase your search!"));
  }
}

void BookSearchBar::setFilter(int index) {
  const QString _leftTr = tr("Search by Booktitle");
  const QString _rightTr = tr("disabled");
  const QVariant _selected = m_selectFilter->itemData(index, Qt::UserRole);

  m_searchLeft->setEnabled(true);
  m_searchLeft->clear();
  m_searchRight->clear();

  switch (qvariant_cast<BookSelectFilter::Filter>(_selected)) {
  case (BookSelectFilter::ArticleId): {
    m_searchLeft->setValidation(SearchLineEdit::Article);
    m_searchLeft->setPlaceholderText(tr("Article Number"));
    m_searchLeft->setToolTip(
        tr("Single Article number or multiple separated by comma."));
    m_searchRight->setEnabled(false);
    m_searchRight->setPlaceholderText(_rightTr);
    setMinLength(2);
  } break;

  case (BookSelectFilter::ISBN): {
    m_searchLeft->setValidation(SearchLineEdit::Numeric);
    m_searchLeft->setPlaceholderText(tr("Search by ISBN number"));
    m_searchRight->setEnabled(false);
    m_searchRight->setPlaceholderText(_rightTr);
    setMinLength(10);
  } break;

  case (BookSelectFilter::BooksAuthor): {
    m_searchLeft->setValidation(SearchLineEdit::Strings);
    m_searchLeft->setPlaceholderText(_leftTr);
    m_searchRight->setValidation(SearchLineEdit::Strings);
    m_searchRight->setEnabled(true);
    m_searchRight->setPlaceholderText(tr("and Authors"));
    setMinLength(p_length);
  } break;

  case (BookSelectFilter::BooksKeyword): {
    m_searchLeft->setValidation(SearchLineEdit::Strings);
    m_searchLeft->setPlaceholderText(_leftTr);
    m_searchRight->setValidation(SearchLineEdit::Strings);
    m_searchRight->setEnabled(true);
    m_searchRight->setPlaceholderText(tr("and Keyword"));
    setMinLength(p_length);
  } break;

  case (BookSelectFilter::Authors): {
    m_searchLeft->setValidation(SearchLineEdit::Strings);
    m_searchLeft->setEnabled(true);
    m_searchLeft->setPlaceholderText(tr("Search by Author"));
    m_searchRight->setValidation(SearchLineEdit::Strings);
    m_searchRight->setEnabled(false);
    m_searchRight->setPlaceholderText(_rightTr);
    setMinLength(p_length);
  } break;

  case (BookSelectFilter::Storage): {
    m_searchLeft->setValidation(SearchLineEdit::Strings);
    m_searchRight->setEnabled(true);
    m_searchLeft->setPlaceholderText(tr("Storage"));
    m_searchRight->setPlaceholderText(tr("and Compartment"));
    setMinLength(2);
  } break;

  default: {
    m_searchLeft->setValidation(SearchLineEdit::Strings);
    m_searchRight->setEnabled(false);
    m_searchLeft->setPlaceholderText(_leftTr);
    m_searchRight->setPlaceholderText(_rightTr);
    setMinLength(p_length);
  } break;
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
  const QStringList _fields = js.value("fields").toString().split(",");
  const QString _operation = js.value("search").toString();
  if (_operation.isEmpty())
    return QString();

  QString _sql(withStock() ? "ib_count>0 AND " : "");

  // Title und Autorensuche
  if (_operation == "title_and_author") {
    _sql.append("(" + getTitleSearch(_fields) + ")");
    return _sql;
  }

  // Buch Titlesuche
  if (_operation == "title") {
    _sql.append("(" + getTitleSearch(_fields) + ")");
    return _sql;
  }

  // Buch Autorensuche
  if (_operation == "author") {
    QString _s = m_searchLeft->getSearch();
    _sql.append("(" + prepareFieldSearch("ib_author", _s) + ")");
    return _sql;
  }

  // Artikel Nummersuche (107368,115110)
  if (_operation == "articleId") {
    QString _s = m_searchLeft->getSearch();
    _s.replace(QRegExp("\\,\\s?$"), "");
    return "ib_id IN (" + _s + ")";
  }

  // ISBN Suche
  if (_operation == "isbn") {
    QString _s = m_searchLeft->getSearch();
    _s.replace(QRegExp("\\D+"), "");
    if (_s.length() == 10 || _s.length() == 13) {
      _sql.append("ib_isbn=" + _s);
      return _sql;
    }
  }

  // Publisher
  if (_operation == "publisher") {
    QString _s = m_searchLeft->getSearch();
    _sql.append("(" + prepareFieldSearch("ib_publisher", _s) + ")");
    return _sql;
  }

  // Lager & Lagefach
  if (_operation == "storage") {
    QString _s = m_searchLeft->getSearch();
    _s.replace(jokerPattern, "%");

    QStringList _left;
    _left << "sl_storage ILIKE '" + _s + "%'";
    if (_s.length() > p_length) {
      _left << prepareFieldSearch("sl_identifier", _s);
      _left << prepareFieldSearch("ib_keyword", _s);
    }
    _sql.append("(" + _left.join(" OR ") + ")");

    QString _c = m_searchRight->getSearch();
    if (_c.length() > p_length) {
      _c.replace(jokerPattern, "%");
      QString _right;
      _right = prepareFieldSearch("ib_storage_compartment", _c);
      _sql.append(" AND (" + _right + ")");
    }

    return _sql;
  }

  qWarning("Not Defined Search (%s)",
           qPrintable(js.value("search").toString()));

  return QString();
}
