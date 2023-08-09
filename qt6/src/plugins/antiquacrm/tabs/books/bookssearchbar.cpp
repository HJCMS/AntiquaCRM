// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookssearchbar.h"
#include "booksselectfilter.h"

#include <QIcon>
#include <QJsonObject>
#include <QRegularExpression>
#include <QToolButton>

BooksSearchBar::BooksSearchBar(QWidget *parent)
    : AntiquaCRM::TabsSearchBar{parent} {
  m_selectFilter = new BooksSelectFilter(this);
  addWidget(m_selectFilter);

  m_searchInput = new AntiquaCRM::ALineEdit(this);
  m_searchInput->setPlaceholderText(tr("Default search"));
  addWidget(m_searchInput);

  QToolButton *m_icontb = new QToolButton(this);
  m_icontb->setEnabled(false);
  m_icontb->setIcon(getIcon("user-group"));
  addWidget(m_icontb);

  m_customSearch = new AntiquaCRM::ALineEdit(this);
  addWidget(m_customSearch);

  addWidget(searchConfines());

  // ib_signed
  m_signed = new QCheckBox(tr("Signed"), this);
  m_signed->setToolTip(tr("Signed Version"));
  addWidget(m_signed);

  addWidget(stockCheckBox());

  m_searchBtn = startSearchButton();
  addWidget(m_searchBtn);

  connect(m_selectFilter, SIGNAL(currentIndexChanged(int)),
          SLOT(setFilter(int)));

  connect(m_searchInput, SIGNAL(returnPressed()), SLOT(setSearch()));
  connect(m_customSearch, SIGNAL(returnPressed()), SLOT(setSearch()));

  connect(m_searchBtn, SIGNAL(clicked()), SLOT(setSearch()));

  setFilter(0);
}

const QString BooksSearchBar::getTitleSearch(const QStringList &fields) {
  QString query;
  QString inputLeft = m_searchInput->text();
  QString inputRight = m_customSearch->text();

  // Standard Suchfeld
  if (m_searchInput->length() >= getMinLength()) {
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
  if (m_customSearch->isEnabled() &&
      m_customSearch->length() >= getMinLength()) {
    QStringList bufferRight;
    if (m_customSearch->placeholderText().contains(tr("Keyword")))
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

bool BooksSearchBar::lineInputsEnabled() {
  return (m_searchInput->isEnabled() || m_customSearch->isEnabled());
}

void BooksSearchBar::setCustomSearch(const QString &info) {
  if (info.length() < 1) {
    m_customSearch->clear();
    m_customSearch->setValidation(m_customSearch->InputValidator::DEFAULT);
    m_customSearch->setEnabled(false);
    m_customSearch->setPlaceholderText(QString());
  } else {
    m_customSearch->clear();
    m_customSearch->setEnabled(true);
    m_customSearch->setPlaceholderText(info);
  }
}

void BooksSearchBar::setSearch() {
  BooksSelectFilter::Filter _filter = m_selectFilter->currentFilter();
  if (_filter == BooksSelectFilter::BOOK_ARTICLE_ID) {
    emit sendSearchClicked();
  } else if (lineInputsEnabled() && requiredLengthExists()) {
    emit sendSearchClicked();
  } else {
    emit sendNotify(tr("Your input is too short, increase your search!"));
  }
}

void BooksSearchBar::setFilter(int index) {
  m_searchInput->clear();
  m_searchInput->setEnabled(true);
  m_searchInput->setToolTip(QString());
  m_customSearch->setToolTip(QString());

  switch (m_selectFilter->currentFilter(index)) {
  case (BooksSelectFilter::BOOK_ARTICLE_ID): {
    m_searchInput->setValidation(
        AntiquaCRM::ALineEdit::InputValidator::ARTICLE);
    m_searchInput->setPlaceholderText(tr("Article number"));
    m_searchInput->setToolTip(
        tr("Multiple Articlenumbers separated by comma."));
    setCustomSearch();
    break;
  }

  case (BooksSelectFilter::BOOK_ISBN): {
    m_searchInput->setValidation(
        AntiquaCRM::ALineEdit::InputValidator::NUMERIC);
    m_searchInput->setPlaceholderText(tr("ISBN search"));
    m_searchInput->setToolTip(tr("Search ISBN number"));
    setCustomSearch();
    break;
  }

  case (BooksSelectFilter::BOOK_TITLE_KEYWORD): {
    m_searchInput->setValidation(
        AntiquaCRM::ALineEdit::InputValidator::STRINGS);
    m_searchInput->setPlaceholderText(tr("Booktitle"));
    m_searchInput->setToolTip(tr("Search Book in title, text fields"));
    setCustomSearch(tr("and Keyword."));
    m_customSearch->setValidation(
        AntiquaCRM::ALineEdit::InputValidator::STRINGS);
    m_customSearch->setToolTip(tr("and Keyword field."));
    break;
  }

  case (BooksSelectFilter::BOOK_TITLE_AUTHOR): {
    m_searchInput->setValidation(
        AntiquaCRM::ALineEdit::InputValidator::STRINGS);
    m_searchInput->setPlaceholderText(tr("Booktitle and Author"));
    m_searchInput->setToolTip(tr("Search Book in title"));
    setCustomSearch(tr("and Author."));
    m_customSearch->setValidation(
        AntiquaCRM::ALineEdit::InputValidator::STRINGS);
    m_customSearch->setToolTip(" " + tr("and Authors."));
    break;
  }

  case (BooksSelectFilter::BOOK_STORAGE): {
    m_searchInput->setValidation(
        AntiquaCRM::ALineEdit::InputValidator::STRINGS);
    m_searchInput->setPlaceholderText(tr("In Storages search"));
    m_searchInput->setToolTip(
        tr("Search with Keyword for Books in Storage locations."));
    setCustomSearch();
    break;
  }

  case (BooksSelectFilter::BOOK_AUTHORS): {
    m_searchInput->setValidation(
        AntiquaCRM::ALineEdit::InputValidator::STRINGS);
    m_searchInput->setPlaceholderText(tr("Authors search"));
    setCustomSearch();
    break;
  }

  case (BooksSelectFilter::BOOK_PUBLISHER): {
    m_searchInput->setValidation(
        AntiquaCRM::ALineEdit::InputValidator::STRINGS);
    m_searchInput->setPlaceholderText(tr("Publishers search"));
    setCustomSearch();
    break;
  }

  default: {
    m_searchInput->setValidation(
        AntiquaCRM::ALineEdit::InputValidator::STRINGS);
    m_searchInput->setPlaceholderText(tr("Unknown"));
    setCustomSearch();
    break;
  }
  };

  emit sendFilterChanged(index);
}

void BooksSearchBar::setFilterFocus() {
  m_selectFilter->setFocus();
  m_selectFilter->showPopup();
}

void BooksSearchBar::setClearAndFocus() {
  m_searchInput->clear();
  m_customSearch->clear();
  m_searchInput->setFocus();
}

void BooksSearchBar::setSearchFocus() { setClearAndFocus(); }

int BooksSearchBar::searchLength() {
  return (m_searchInput->length() + m_customSearch->length());
}

bool BooksSearchBar::requiredLengthExists() {
  return (searchLength() >= getMinLength());
}

const QString BooksSearchBar::getSearchStatement() {
  static const QRegularExpression stringListPattern("\\,\\s?$");
  static const QRegularExpression numericPattern("\\D+");
  QJsonObject _js = m_selectFilter->getFilter(m_selectFilter->currentIndex());
  QString _sql(withStock() ? "ib_count>0 AND " : "");
  QStringList _columns = _js.value("fields").toString().split(",");
  QString _operation = _js.value("search").toString();
  QString _input;

  // ib_signed
  if (m_signed->isChecked()) {
    _sql.append("ib_signed=true AND ");
  }

  if (_operation.isEmpty())
    return QString();

  // Title und Autorensuche
  if (_operation == "title_and_author") {
    _sql.append("(" + getTitleSearch(_columns) + ")");
    return _sql;
  }

  // Buch Titlesuche
  if (_operation == "title") {
    _sql.append("(" + getTitleSearch(_columns) + ")");
    return _sql;
  }

  // Buch Autorensuche
  if (_operation == "author") {
    _sql.append("(" + getTitleSearch(_columns) + ")");
    return _sql;
  }

  // Artikel Nummersuche (107368,115110)
  if (_operation == "articleId") {
    _input = m_searchInput->text();
    _input.replace(stringListPattern, "");
    return "ib_id IN (" + _input + ")";
  }

  // ISBN Suche
  if (_operation == "isbn") {
    _input = m_searchInput->text();
    _input.replace(numericPattern, "");
    if (_input.length() == 10 || _input.length() == 13) {
      _sql.append("ib_isbn=" + _input);
      return _sql;
    }
  }

  // Publisher
  if (_operation == "publisher") {
    _input = m_searchInput->text();
    _sql.append("(" + prepareFieldSearch("ib_publisher", _input) + ")");
    return _sql;
  }

  // Lager & Stichwortsuche
  if (_operation == "storage") {
    _input = m_searchInput->text();
    _input.replace(jokerPattern, "%");
    QStringList _buf;
    _buf << prepareFieldSearch("sl_storage", _input);
    _buf << prepareFieldSearch("sl_identifier", _input);
    _buf << prepareFieldSearch("ib_keyword", _input);
    _sql.append("(" + _buf.join(" OR ") + ")");
    _buf.clear();
    return _sql;
  }

  qWarning("Not Defined Search (%s)",
           qPrintable(_js.value("search").toString()));

  return QString();
}
