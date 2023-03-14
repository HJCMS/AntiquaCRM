// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "cdvsearchbar.h"
#include "cdvfilterselect.h"
#include "cdvsearchline.h"

#include <QDebug>

CDVSearchBar::CDVSearchBar(QWidget *parent) : TabSearchBar{parent} {
  setObjectName("printsstitches_search_bar");

  m_selectFilter = new CDVFilterSelect(this);
  QString filterTip = tr("Press CTRL+Shift+F, to quickly open this Menu.");
  m_selectFilter->setToolTip(filterTip);
  addWidget(m_selectFilter);

  m_searchEdit = new CDVSearchLine(this);
  m_searchEdit->setPlaceholderText(tr("Search Title"));
  addWidget(m_searchEdit);

  addWidget(searchConfines());
  addSeparator();
  addWidget(stockCheckBox());

  m_searchBtn = startSearchButton();
  addWidget(m_searchBtn);

  connect(m_selectFilter, SIGNAL(currentIndexChanged(int)),
          SLOT(setFilter(int)));
  connect(m_searchEdit, SIGNAL(returnPressed()), SLOT(setSearch()));
  connect(m_searchBtn, SIGNAL(clicked()), SLOT(setSearch()));
}

void CDVSearchBar::setSearch() {
  if (m_searchEdit->getLength() < getMinLength()) {
    emit sendNotify(tr("Your input is too short, increase your search!"));
    return;
  }

  p_search = QString();
  QString left = m_searchEdit->getSearch();
  left = left.replace(quotePattern, "");
  left = left.replace(trimPattern, " ");
  p_search = left.trimmed();
  emit sendSearchClicked();
}

void CDVSearchBar::setFilter(int index) {
  QVariant val = m_selectFilter->itemData(index, Qt::UserRole);
  CDVFilterSelect::CDVFilter filter =
      static_cast<CDVFilterSelect::CDVFilter>(val.toInt());

  QString tip;
  QStringList keys({tr("Genres"), tr("Keywords"), tr("Description")});

  switch (filter) {
  case CDVFilterSelect::CDVFilter::CDV_TITLE:
    tip = tr("Search Album title.");
    m_searchEdit->setValidation(SearchLineEdit::Strings);
    m_searchEdit->setPlaceholderText(tip);
    m_searchEdit->setToolTip(tip);
    break;

  case CDVFilterSelect::CDVFilter::CDV_KEYWORDS:
    tip = tr("Search in") + " " + keys.join(", ");
    m_searchEdit->setValidation(SearchLineEdit::Strings);
    m_searchEdit->setPlaceholderText(tip);
    m_searchEdit->setToolTip(tip);
    break;

  case CDVFilterSelect::CDVFilter::CDV_ARTICLE:
    tip = tr("Search with Article number.");
    m_searchEdit->setValidation(SearchLineEdit::Article);
    m_searchEdit->setPlaceholderText(tip);
    m_searchEdit->setToolTip(tip);
    break;

  case CDVFilterSelect::CDVFilter::CDV_ISBNEAN:
    tip = tr("Search with Barcode a multimedia article.");
    m_searchEdit->setValidation(SearchLineEdit::Article);
    m_searchEdit->setPlaceholderText(tip);
    m_searchEdit->setToolTip(tip);
    break;

  default:
    tip = tr("Search for Artists, Group or Songwriters.");
    m_searchEdit->setValidation(SearchLineEdit::Strings);
    m_searchEdit->setPlaceholderText(tip);
    m_searchEdit->setToolTip(tip);
    break;
  };

  setClearAndFocus();
  emit sendFilterChanged(index);
}

void CDVSearchBar::setFilterFocus() {
  m_selectFilter->setFocus();
  m_selectFilter->showPopup();
}

void CDVSearchBar::setClearAndFocus() {
  m_searchEdit->clear();
  m_searchEdit->setFocus();
}

void CDVSearchBar::setSearchFocus() { setClearAndFocus(); }

int CDVSearchBar::searchLength() { return m_searchEdit->getSearch().length(); }

const QString CDVSearchBar::getSearchStatement() {
  if (!m_searchEdit->isValid(getMinLength()))
    return QString();

  QJsonObject js = m_selectFilter->getFilter(m_selectFilter->currentIndex());
  QString search = m_searchEdit->getSearch();
  QString sql(withStock() ? "cv_count>0 AND " : "");

  // Artikel Nummernsuche
  if (js.value("search").toString() == "articleId") {
    search.replace(QRegExp("\\,\\s?$"), "");
    sql = QString("cv_id IN (" + search + ")");
    return sql;
  }

  // Titlesuche
  if (js.value("search").toString() == "title") {
    QStringList s_pattern;
    s_pattern << prepareFieldSearch("cv_title", search);
    s_pattern << prepareFieldSearch("cv_title_extended", search);
    sql.append(s_pattern.join(" OR "));
    return sql;
  }

  // Künstlersuche
  if (js.value("search").toString() == "artists") {
    QStringList s_pattern;
    s_pattern << prepareFieldSearch("cv_author", search);
    s_pattern << prepareFieldSearch("cv_publisher", search);
    sql.append(s_pattern.join(" OR "));
    return sql;
  }

  // Barcode
  if (js.value("search").toString() == "barcode") {
    sql.append("cv_eangtin='" + search + "'");
    return sql;
  }

  // Schlüsselwort & Genre suche
  if (js.value("search").toString() == "genres_keywords") {
    QStringList buffer;
    QStringList fields = js.value("fields").toString().split(",");
    foreach (QString f, fields) {
      if (f == "cv_keyword")
        buffer.append(prepareFieldSearch(f, search));
      else
        buffer.append(f + " ILIKE '%" + search + "%'");
    }
    sql.append(buffer.join(" OR "));
    return sql;
  }

  qWarning("Not Defined Search (%s)",
           qPrintable(js.value("search").toString()));

  return QString();
}
