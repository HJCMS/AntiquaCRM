// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderssearchbar.h"

#include <AGlobal>
#include <QIcon>

OrdersSearchBar::OrdersSearchBar(QWidget *parent) : TabSearchBar{parent} {
  setObjectName("orders_search_bar");
  searchField = QString();
  searchText = QString();

  QIcon icon(":icons/view_search.png");
  m_selectFilter = new AntiquaComboBox(this);
  QString filterTip = tr("Press CTRL+Shift+F, to quickly open this Menu.");
  m_selectFilter->setToolTip(filterTip);
  m_selectFilter->addItem(icon, tr("Order Id"), "o_id");
  m_selectFilter->addItem(icon, tr("Customer or Company"), "customer");
  m_selectFilter->addItem(icon, tr("Delivery Service"), "d_name");
  m_selectFilter->addItem(icon, tr("Provider"), "o_provider_name");
  addWidget(m_selectFilter);

  m_lineEdit = new SearchLineEdit(this);
  m_lineEdit->setValidation(SearchLineEdit::Strings);
  addWidget(m_lineEdit);

  m_searchBtn = clickSearchButton(tr("Search"));
  m_searchBtn->setToolTip(tr("Input search"));
  addWidget(m_searchBtn);

  addSeparator();

  m_restoreView = new QPushButton(tr("Default view"));
  m_restoreView->setIcon(QIcon("://icons/spreadsheet.png"));
  m_restoreView->setToolTip(tr("Load the Standard table view."));
  addWidget(m_restoreView);

  connect(m_selectFilter, SIGNAL(currentIndexChanged(int)),
          SLOT(setFilter(int)));
  connect(m_lineEdit, SIGNAL(returnPressed()), SLOT(setSearch()));
  connect(m_lineEdit, SIGNAL(returnPressed()), SLOT(setSearch()));
  connect(m_searchBtn, SIGNAL(clicked()), SLOT(setSearch()));
  connect(m_restoreView, SIGNAL(clicked()), SIGNAL(sendRestoreView()));

  m_selectFilter->setCurrentIndex(1);
}

void OrdersSearchBar::setSearch() {
  QString search = m_lineEdit->text().trimmed();
  if (search.isEmpty()) {
    return;
  }

  if (searchField.isEmpty()) {
    searchField =
        m_selectFilter->itemData(m_selectFilter->currentIndex(), Qt::UserRole)
            .toString();
  }

  search = search.replace(jokerPattern, " ");
  search = search.replace(quotePattern, "");
  search = search.replace(trimPattern, "");
  searchText = search;

  emit sendSearchClicked();
}

void OrdersSearchBar::setFilter(int index) {
  searchField = m_selectFilter->itemData(index, Qt::UserRole).toString();
  QString txt = m_selectFilter->itemData(index, Qt::DisplayRole).toString();
  m_lineEdit->setPlaceholderText(tr("Search for %1").arg(txt));
  if (searchField == "o_id")
    m_lineEdit->setValidation(SearchLineEdit::Article);
  else
    m_lineEdit->setValidation(SearchLineEdit::Strings);

  setClearAndFocus();
  emit sendFilterChanged(index);
}

void OrdersSearchBar::setFilterFocus() {
  m_selectFilter->setFocus();
  m_selectFilter->showPopup();
}

void OrdersSearchBar::setClearAndFocus() {
  m_lineEdit->clear();
  m_lineEdit->setFocus();
}

void OrdersSearchBar::setSearchFocus() { setClearAndFocus(); }

int OrdersSearchBar::searchLength() { return searchText.length(); }

const QString OrdersSearchBar::getSearchStatement() {
  if (searchField == "o_id") {
    return QString("o_id='%1'").arg(searchText);
  }

  if (searchField == "customer") {
    QStringList sList({"c_company_name", "c_firstname", "c_lastname"});
    QStringList buffer;
    foreach (QString f, sList) {
      buffer << f + " ILIKE '%" + searchText + "%'";
    }
    return buffer.join(" OR ");
  }

  QString out(searchField);
  out.append(" ILIKE '%");
  out.append(searchText);
  out.append("%'");
  return out;
}
