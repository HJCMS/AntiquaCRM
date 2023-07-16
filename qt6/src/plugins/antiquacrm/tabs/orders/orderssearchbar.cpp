// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderssearchbar.h"

#include <QToolButton>

OrdersSearchBar::OrdersSearchBar(QWidget *parent)
    : AntiquaCRM::TabsSearchBar{parent} {

  const QIcon _icon = AntiquaCRM::antiquaIcon("action-search");
  const QString _tip = tr("Press CTRL+Shift+F, to quickly open this Menu.");

  m_filter = new AntiquaCRM::AComboBox(this);
  m_filter->setToolTip(_tip);
  m_filter->addItem(_icon, tr("Order Id"));            // o_id
  m_filter->addItem(_icon, tr("Customer or Company")); // customer
  m_filter->addItem(_icon, tr("Delivery Service"));    // d_name
  m_filter->addItem(_icon, tr("Provider"));            // o_provider_name
  m_filter->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  addWidget(m_filter);

  QToolButton *m_icontb = new QToolButton(this);
  m_icontb->setEnabled(false);
  m_icontb->setIcon(AntiquaCRM::antiquaIcon("action-search"));
  addWidget(m_icontb);

  m_searchInput = new AntiquaCRM::ALineEdit(this);
  m_searchInput->setPlaceholderText(tr("Search Order id"));
  m_searchInput->setValidation(AntiquaCRM::ALineEdit::InputValidator::ARTICLE);
  addWidget(m_searchInput);

  m_datePart = new AntiquaCRM::AComboBox(this);
  m_datePart->setToolTip(tr("Restrict search to current to selection."));
  m_datePart->addItem(_icon, tr("Year"), "year");
  m_datePart->addItem(_icon, tr("Month"), "month");
  m_datePart->addItem(_icon, tr("Week"), "week");
  m_datePart->addItem(_icon, tr("Day"), "day");
  m_datePart->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  addWidget(m_datePart);

  m_searchBtn = startSearchButton();
  addWidget(m_searchBtn);

  connect(m_filter, SIGNAL(currentIndexChanged(int)), SLOT(setFilter(int)));
  connect(m_searchInput, SIGNAL(returnPressed()), SLOT(setSearch()));
  connect(m_searchBtn, SIGNAL(clicked()), SLOT(setSearch()));
}

const QString OrdersSearchBar::getDatePart() const {
  QString _sql("(");
  QString _dp = m_datePart->itemData(m_datePart->currentIndex()).toString();
  _sql.append("DATE_PART('" + _dp + "',o_since)=");
  _sql.append("DATE_PART('" + _dp + "',CURRENT_DATE)");
  if (_dp == "year") {
    _sql.append(")");
    return _sql;
  }
  // append month to day query
  if (_dp == "day") {
    _sql.append(" AND ");
    _sql.append("DATE_PART('month',o_since)=");
    _sql.append("DATE_PART('month',CURRENT_DATE)");
  }
  // append always year
  _sql.append(" AND ");
  _sql.append("DATE_PART('year',o_since)=DATE_PART('year',CURRENT_DATE)");
  _sql.append(")");
  return _sql;
}

void OrdersSearchBar::setSearch() {
  QString _search = m_searchInput->text().trimmed();
  _search.replace("'", "’");
  _search = _search.replace(jokerPattern, " ");
  _search = _search.replace(quotePattern, "");
  _search = _search.replace(trimPattern, "");
  p_search = _search;
  emit sendSearchClicked();
}

void OrdersSearchBar::setFilter(int index) {
  switch (index) {
  case 0: {
    m_searchInput->setPlaceholderText(tr("Search Order id"));
    m_searchInput->setValidation(
        AntiquaCRM::ALineEdit::InputValidator::ARTICLE);
  } break;

  case 1: {
    m_searchInput->setPlaceholderText(tr("Search Customer or Company"));
    m_searchInput->setValidation(
        AntiquaCRM::ALineEdit::InputValidator::STRINGS);
  } break;

  case 2: {
    m_searchInput->setPlaceholderText(tr("Search Delivery Service"));
    m_searchInput->setValidation(
        AntiquaCRM::ALineEdit::InputValidator::STRINGS);
  } break;

  case 3: {
    m_searchInput->setPlaceholderText(tr("Search Provider"));
    m_searchInput->setValidation(
        AntiquaCRM::ALineEdit::InputValidator::STRINGS);
  } break;

  default:
    m_searchInput->setPlaceholderText(tr("Search ..."));
    m_searchInput->setValidation(
        AntiquaCRM::ALineEdit::InputValidator::NOTHING);
    break;
  }
  setClearAndFocus();
  emit sendFilterChanged(index);
}

void OrdersSearchBar::setFilterFocus() {
  m_filter->setFocus();
  m_filter->showPopup();
}

void OrdersSearchBar::setClearAndFocus() {
  m_searchInput->clear();
  setSearchFocus();
}

void OrdersSearchBar::setSearchFocus() { m_searchInput->setFocus(); }

int OrdersSearchBar::searchLength() {
  const QString _search = m_searchInput->text().trimmed();
  return _search.length();
}

const QString OrdersSearchBar::getSearchStatement() {
  int _index = m_filter->currentIndex();
  QString _sql;
  switch (_index) {
  case 0:
    _sql = QString("o_id IN (%1)").arg(p_search);
    break;

  case 1: {
    QStringList sList({"c_company_name", "c_firstname", "c_lastname"});
    QStringList buffer;
    foreach (QString f, sList) {
      buffer << f + " ILIKE '%" + p_search + "%'";
    }
    _sql = "(" + buffer.join(" OR ") + ")";
    _sql.append(" AND " + getDatePart());
  } break;

  case 2:
    _sql = QString("d_name ILIKE '%1%'").arg(p_search);
    _sql.append(" AND " + getDatePart());
    break;

  case 3:
    _sql = QString("o_provider_name ILIKE '%1%'").arg(p_search);
    _sql.append(" AND " + getDatePart());
    break;

  default:
    _sql.clear();
    break;
  }

  qDebug() << Q_FUNC_INFO << _sql;
  return _sql;
}
