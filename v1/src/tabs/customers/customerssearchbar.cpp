// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customerssearchbar.h"
#include "customersselectfilter.h"

CustomersSearchBar::CustomersSearchBar(QWidget *parent) : TabSearchBar{parent} {
  setObjectName("customers_search_bar");

  m_selectFilter = new CustomersSelectFilter(this);
  QString filterTip = tr("Press CTRL+Shift+F, to quickly open this Menu.");
  m_selectFilter->setToolTip(filterTip);
  addWidget(m_selectFilter);

  m_searchEdit = new SearchLineEdit(this);
  m_searchEdit->setPlaceholderText(tr("Search customers"));
  QString searchTip = tr("Press CTRL+Shift+S to focus in.");
  m_searchEdit->setToolTip(searchTip);
  addWidget(m_searchEdit);

  m_searchBtn = startSearchButton(tr("Search customer"));
  addWidget(m_searchBtn);

  addSeparator();

  addWidget(defaultViewButton());

  connect(m_selectFilter, SIGNAL(currentIndexChanged(int)),
          SLOT(setFilter(int)));
  connect(m_searchEdit, SIGNAL(returnPressed()), SLOT(setSearch()));
  connect(m_searchBtn, SIGNAL(clicked()), SLOT(setSearch()));
}

void CustomersSearchBar::setSearch() {
  if (m_searchEdit->getLength() < minLength)
    return;

  emit sendSearchClicked();
}

void CustomersSearchBar::setFilter(int index) {
  QVariant val = m_selectFilter->itemData(index, Qt::UserRole);
  CustomersSelectFilter::Filter filter =
      qvariant_cast<CustomersSelectFilter::Filter>(val);
  switch (filter) {
  case (CustomersSelectFilter::CustomerId): {
    m_searchEdit->setPlaceholderText(tr("Search with customer id"));
    m_searchEdit->setValidation(SearchLineEdit::Numeric);
    break;
  }

  case (CustomersSelectFilter::Company): {
    m_searchEdit->setPlaceholderText(tr("Search company"));
    m_searchEdit->setValidation(SearchLineEdit::Strings);
    break;
  }

  default:
    m_searchEdit->setPlaceholderText(tr("Search customer"));
    m_searchEdit->setValidation(SearchLineEdit::Strings);
    break;
  };
}

void CustomersSearchBar::setFilterFocus() {
  m_selectFilter->setFocus();
  m_selectFilter->showPopup();
}

void CustomersSearchBar::setClearAndFocus() {
  m_searchEdit->clear();
  m_searchEdit->setFocus();
}

void CustomersSearchBar::setSearchFocus() { setClearAndFocus(); }

int CustomersSearchBar::searchLength() { return m_searchEdit->text().length(); }

const QString CustomersSearchBar::getSearchStatement() {
  CustomersSelectFilter::Filter filter = m_selectFilter->currentFilter();
  QJsonObject js = m_selectFilter->getFilter(filter);
  QString searchLine = m_searchEdit->text().trimmed();
  QStringList fields = js.value("fields").toString().split(",");
  QStringList buffer;

  // Kunden Nummernsuche
  if (js.value("search").toString() == "customer_id") {
    searchLine.replace(QRegExp("\\,\\s?$"), "");
    return "c_id IN (" + searchLine + ")";
  }

  QStringList words(searchLine);
  QRegExp spaces("[\\s\\t]+");
  if (searchLine.contains(spaces)) {
    words = searchLine.split(spaces);
  }

  // Unternehmen oder Organisation
  if (js.value("search").toString() == "customer_company_name") {
    foreach (QString f, fields) {
      if(words.size()>1) {
        foreach (QString s, words) {
          buffer << f + " ILIKE '" + s + "%'";
        }
      } else {
        buffer << f + " ILIKE '%" + searchLine + "%'";
      }
    }
    return buffer.join(" OR ");
  }

  // Kundensuche
  foreach (QString f, fields) {
    foreach (QString s, words) {
      buffer << f + " ILIKE '" + s + "%'";
    }
  }
  return buffer.join(" OR ");
}
