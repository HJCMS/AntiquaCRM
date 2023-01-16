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

  m_searchBtn = startSearchButton(tr("Search"));
  addWidget(m_searchBtn);

  addSeparator();

  addWidget(defaultViewButton());

  connect(m_selectFilter, SIGNAL(currentIndexChanged(int)),
          SLOT(setFilter(int)));
  connect(m_searchEdit, SIGNAL(returnPressed()), SLOT(setSearch()));
  connect(m_searchBtn, SIGNAL(clicked()), SLOT(setSearch()));
}

void CDVSearchBar::setSearch() {
  if (m_searchEdit->getLength() <= minLength)
    return;

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

  QString se(tr("Search") + " ");

  switch (filter) {
  case CDVFilterSelect::CDVFilter::CDV_TITLE:
    m_searchEdit->setValidation(SearchLineEdit::Strings);
    m_searchEdit->setPlaceholderText(se + tr("Title"));
    break;

  case CDVFilterSelect::CDVFilter::CDV_KEYWORDS:
    m_searchEdit->setValidation(SearchLineEdit::Strings);
    m_searchEdit->setPlaceholderText(se + tr("Keyword"));
    break;

  case CDVFilterSelect::CDVFilter::CDV_AUTHOR:
    m_searchEdit->setValidation(SearchLineEdit::Strings);
    m_searchEdit->setPlaceholderText(se + tr("Author"));
    break;

  case CDVFilterSelect::CDVFilter::CDV_ARTICLE:
    m_searchEdit->setValidation(SearchLineEdit::Article);
    m_searchEdit->setPlaceholderText(se + tr("Article number"));
    break;

  case CDVFilterSelect::CDVFilter::CDV_ISBNEAN:
    m_searchEdit->setValidation(SearchLineEdit::Article);
    m_searchEdit->setPlaceholderText(se + tr("ISBN/EAN number"));
    break;

  default:
    m_searchEdit->setValidation(SearchLineEdit::Strings);
    m_searchEdit->setPlaceholderText(se + tr("Title"));
    break;
  };

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
  QJsonObject js = m_selectFilter->getFilter(m_selectFilter->currentIndex());
  QStringList fields = js.value("fields").toString().split(",");
  QString search = m_searchEdit->getSearch();
  // Artikel Nummernsuche (107368,115110)
  if (js.value("search").toString() == "articleId") {
    search.replace(QRegExp("\\,\\s?$"), "");
    return "cv_id IN (" + search + ")";
  }

  // Titlesuche
  if (js.value("search").toString() == "title") {
    QString sql("cv_title ILIKE '%" + search + "%' OR ");
    sql.append("cv_title_extended ILIKE '%" + search + "%'");
    return sql;
  }

  // Autorensuche
  if (js.value("search").toString() == "author") {
    QString sql("cv_author ILIKE '%" + search + "%' OR ");
    sql.append("cv_description ILIKE '%" + search + "%'");
    return sql;
  }

  // ISBN/EAN
  if (js.value("search").toString() == "isbn") {
    QString sql("cv_isbn ILIKE '" + search + "'");
    return sql;
  }

  // Schlüsselwort suche
  if (js.value("search").toString() == "keywords") {
    QStringList buffer;
    QString sql;
    foreach (QString f, fields) {
      buffer.append(f + " ILIKE '%" + search + "%'");
    }
    sql.append(buffer.join(" OR "));
    return sql;
  }

  qWarning("Not Defined Search (%s)",
           qPrintable(js.value("search").toString()));

  return QString();
}