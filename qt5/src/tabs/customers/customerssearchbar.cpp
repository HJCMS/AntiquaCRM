// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customerssearchbar.h"
#include "customerssearchlineedit.h"
#include "customersselectfilter.h"

CustomersSearchBar::CustomersSearchBar(QWidget *parent) : TabSearchBar{parent} {
  setObjectName("customers_search_bar");

  m_selectFilter = new CustomersSelectFilter(this);
  QString filterTip = tr("Press CTRL+Shift+F, to quickly open this Menu.");
  m_selectFilter->setToolTip(filterTip);
  addWidget(m_selectFilter);

  m_searchEdit = new CustomersSearchLineEdit(this);
  addWidget(m_searchEdit);

  m_searchBtn = startSearchButton();
  addWidget(m_searchBtn);

  connect(m_selectFilter, SIGNAL(currentIndexChanged(int)),
          SLOT(setFilter(int)));
  connect(m_searchEdit, SIGNAL(returnPressed()), SLOT(setSearch()));
  connect(m_searchBtn, SIGNAL(clicked()), SLOT(setSearch()));
}

void CustomersSearchBar::setSearch() {
  if (m_searchEdit->getLength() < getMinLength()) {
    emit sendNotify(tr("Your input is too short, increase your search!"));
    return;
  }
  emit sendSearchClicked();
}

void CustomersSearchBar::setFilter(int index) {
  switch (m_selectFilter->filterIndex(index)) {
  case (CustomersSelectFilter::CustomerId): {
    m_searchEdit->setPlaceholderText(tr("Search with customer id"));
    m_searchEdit->setValidation(SearchLineEdit::Article);
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

  setClearAndFocus();
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

int CustomersSearchBar::searchLength() {
  return m_searchEdit->getSearch().length();
}

const QString CustomersSearchBar::getSearchStatement() {
  QString _input = m_searchEdit->getSearch();
  QJsonObject _js = m_selectFilter->getFilter(m_selectFilter->filterIndex());
  QStringList _fields = _js.value("fields").toString().split(",");
  QString _search = _js.value("search").toString();

  // Kunden Nummernsuche
  if (_search == "customer_id") {
    _input.replace(QRegExp("\\,\\s?$"), "");
    return "c_id IN (" + _input + ")";
  }

  QStringList _buffer;
  QStringList _statement(_input);
  if (_input.contains(m_searchEdit->spacePattern())) {
    _statement = _input.split(m_searchEdit->spacePattern());
  }

  // Unternehmen oder Organisation
  if (_search == "customer_company_name") {
    foreach (QString f, _fields) {
      if (_statement.size() > 1) {
        foreach (QString s, _statement) {
          _buffer << f + " ILIKE '" + s + "%'";
        }
      } else {
        _buffer << f + " ILIKE '%" + _input + "%'";
      }
    }
    return _buffer.join(" OR ");
  }

  // Kundensuche
  foreach (QString f, _fields) {
    foreach (QString s, _statement) {
      _buffer << f + " ILIKE '" + s + "%'";
    }
  }

  return _buffer.join(" OR ");
}
