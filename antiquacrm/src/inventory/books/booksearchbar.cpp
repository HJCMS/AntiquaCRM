// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booksearchbar.h"
#include "antiqua_global.h"
#include "bookstatements.h"
#include "myicontheme.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonValue>
#include <QToolButton>

SearchFilterBox::SearchFilterBox(QWidget *parent) : QComboBox{parent} {
  setSizeAdjustPolicy(QComboBox::AdjustToContents);
  QIcon icon(myIcon("search"));
  insertItem(0, icon, tr("Book Title and Author"), Filter::BooksAuthor);
  setItemData(0, tr("Search in Book titles and authors"), Qt::ToolTipRole);

  insertItem(1, icon, tr("Book Title and Keyword"), Filter::BooksKeyword);
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
  case (Filter::BooksAuthor): {
    obj.insert("search", QJsonValue("title_and_author"));
    obj.insert("fields", QJsonValue("ib_title,ib_title_extended,ib_author"));
    break;
  }

  case (Filter::BooksKeyword): {
    obj.insert("search", QJsonValue("title"));
    obj.insert("fields", QJsonValue("ib_title,ib_title_extended,ib_keyword"));
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
  m_textCodec = QTextCodec::codecForName("UTF-8");
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

const QString SearchLineEdit::utf8() {
  // TODO
  QString txt = text().trimmed();
  return txt.replace("'","");
}

BookSearchBar::BookSearchBar(QWidget *parent) : QToolBar{parent} {
  setObjectName("book_search_bar");
  setOrientation(Qt::Horizontal);
  setFloatable(false);
  setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

  m_filterSection = new SearchFilterBox(this);
  QString filterTip = tr("Press CTRL+Shift+F, to quickly open this Menu.");
  m_filterSection->setToolTip(filterTip);
  addWidget(m_filterSection);
  addSeparator();

  m_searchLeft = new SearchLineEdit(this);
  QString searchTip = tr("Press CTRL+Shift+S, it clears the input for a new search.");
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

  connect(ac_search, SIGNAL(triggered()), this, SLOT(prepareSearchQuery()));
}

const QString BookSearchBar::prepareFieldSet(const QString &fieldname,
                                             const QString &search) const {
  QString sf(search);
  sf = sf.replace(stripJokers, "%");
  QStringList words = search.split(" ");
  QString sql(fieldname);
  sql.append(" ILIKE '%");
  if (words.count() == 2) {
    sql.append(words.first());
    sql.append("%");
    sql.append(words.last());
    sql.append("%' OR ");
    sql.append(fieldname);
    sql.append(" ILIKE '");
    sql.append(words.last());
    sql.append("%");
    sql.append(words.first());
  } else {
    sql.append(sf);
  }
  sql.append("%'");
  return sql;
}

const QString BookSearchBar::getTitleSearch(const QStringList &fields) {
  QString query;
  // Standard Suchfeld
  if (m_searchLeft->utf8().length() >= 2) {
    QStringList bufferLeft;
    foreach (QString f, fields) {
      if (f != "ib_author")
        bufferLeft << prepareFieldSet(f, p_currentLeft);
    }
    query.append("(");
    query.append(bufferLeft.join(" OR "));
    query.append(")");
  }
  // Autoren Suchfeld
  if (m_searchRight->isEnabled() && m_searchRight->utf8().length() >= 2) {
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
  }
  return query;
}

void BookSearchBar::filterChanged(int index) {
  QVariant val = m_filterSection->itemData(index, Qt::UserRole);
  SearchFilterBox::Filter f = qvariant_cast<SearchFilterBox::Filter>(val);
  m_searchLeft->setEnabled(true);
  m_searchLeft->clear();
  m_searchRight->clear();

  QString leftTr = tr("Search Booktitle");
  QString rightTr = tr("Authors search");

  switch (f) {
  case (SearchFilterBox::ArticleId): {
    m_searchLeft->setValidation(SearchLineEdit::Article);
    m_searchLeft->setPlaceholderText(
        tr("Single Article number or multiple separated by comma."));
    m_searchRight->setEnabled(false);
    m_searchRight->setPlaceholderText(rightTr);
    break;
  }

  case (SearchFilterBox::ISBN): {
    m_searchLeft->setValidation(SearchLineEdit::Numeric);
    m_searchLeft->setPlaceholderText(tr("Search by ISBN number"));
    m_searchRight->setEnabled(false);
    m_searchRight->setPlaceholderText(rightTr);
    break;
  }

  case (SearchFilterBox::BooksAuthor): {
    m_searchLeft->setValidation(SearchLineEdit::Strings);
    m_searchLeft->setPlaceholderText(leftTr);
    m_searchRight->setValidation(SearchLineEdit::Strings);
    m_searchRight->setEnabled(true);
    m_searchRight->setPlaceholderText(tr("and Authors"));
    break;
  }

  case (SearchFilterBox::BooksKeyword): {
    m_searchLeft->setValidation(SearchLineEdit::Strings);
    m_searchLeft->setPlaceholderText(leftTr);
    m_searchRight->setValidation(SearchLineEdit::Strings);
    m_searchRight->setEnabled(true);
    m_searchRight->setPlaceholderText(tr("Keyword"));
    break;
  }

  case (SearchFilterBox::Authors): {
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
  emit currentFilterChanged(index);
}

void BookSearchBar::prepareSearchQuery() {
  if (m_searchLeft->isEnabled() && m_searchLeft->utf8().length() > minLength) {
    QString left = m_searchLeft->utf8();
    left = left.replace(stripPattern, "");
    left = left.replace(trimPattern, " ");
    p_currentLeft = left.trimmed();
  } else {
    p_currentLeft = QString();
  }

  if (m_searchRight->isEnabled() &&
      m_searchRight->utf8().length() > minLength) {
    QString right = m_searchRight->utf8();
    right = right.replace(stripPattern, "");
    right = right.replace(trimPattern, " ");
    p_currentRight = right.trimmed();
  } else {
    p_currentRight = QString();
  }
  emit searchClicked();
}

void BookSearchBar::clearAndFocus() {
  m_searchLeft->clear();
  m_searchRight->clear();
  m_searchLeft->setFocus();
}

void BookSearchBar::setSearchFocus() { clearAndFocus(); }

void BookSearchBar::setFilterFocus() {
  m_filterSection->setFocus();
  m_filterSection->showPopup();
}

int BookSearchBar::searchLength() {
  if (m_searchLeft->isEnabled())
    return m_searchLeft->utf8().length();
  else
    return m_searchRight->utf8().length();
}

const QString BookSearchBar::getSearchStatement() {
  QJsonObject js = m_filterSection->getFilter(m_filterSection->currentIndex());
  QStringList fields = js.value("fields").toString().split(",");
  QString query;
  // Title und Autorensuche
  if (js.value("search").toString() == "title_and_author") {
    query = getTitleSearch(fields);
    return query;
  }
  // Buch Titlesuche
  if (js.value("search").toString() == "title") {
    query = getTitleSearch(fields);
    return query;
  }
  // Buch Autorensuche
  if (js.value("search").toString() == "author") {
    query = getTitleSearch(fields);
    return query;
  }
  // Artikel Nummersuche (107368,115110)
  if (js.value("search").toString() == "articleId") {
    QString s = m_searchLeft->utf8();
    s.replace(articlePattern, "");
    query = "ib_id IN (" + s + ")";
    return query;
  }
  // ISBN Suche
  if (js.value("search").toString() == "isbn") {
    QString s = m_searchLeft->utf8();
    s.replace(isbnPattern, "");
    query = "ib_isbn=" + s;
    return query;
  }
  // Publisher
  if (js.value("search").toString() == "publisher") {
    QString s = m_searchLeft->utf8();
    s.replace(stripJokers, "%");
    query = "ib_publisher ILIKE '" + s + "%'";
    return query;
  }
  // Lager & Stichwortsuche
  if (js.value("search").toString() == "storage") {
    QString s = m_searchLeft->utf8();
    s.replace(stripJokers, "%");
    query = ("ib_count>0 AND (sl_storage ILIKE '");
    query.append(s + "' OR sl_identifier ILIKE '" + s + "%')");
    query.append(" OR (ib_keyword ILIKE '" + s + "%')");
    return query;
  }
  qWarning("Not Defined Search (%s)",
           qPrintable(js.value("search").toString()));
  return QString();
}
