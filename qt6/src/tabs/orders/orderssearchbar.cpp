// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderssearchbar.h"

#include <QFrame>
#include <QToolButton>

OrdersSearchBar::OrdersSearchBar(QWidget *parent)
    : AntiquaCRM::TabsSearchBar{parent} {

  const QIcon _icon = AntiquaCRM::antiquaIcon("view-search");
  const QString _tip = tr("Press CTRL+Shift+F, to quickly open this Menu.");

  m_filter = new AntiquaCRM::AComboBox(this);
  m_filter->setToolTip(_tip);
  m_filter->addItem(_icon, tr("Recipient"));        // customer
  m_filter->addItem(_icon, tr("Order Id"));         // o_id
  m_filter->addItem(_icon, tr("Delivery Service")); // d_name
  m_filter->addItem(_icon, tr("Provider"));         // o_provider_name
  addWidget(m_filter);

  QToolButton *m_icontb = new QToolButton(this);
  m_icontb->setEnabled(false);
  m_icontb->setIcon(AntiquaCRM::antiquaIcon("view-search"));
  addWidget(m_icontb);

  m_searchInput = new AntiquaCRM::ALineEdit(this);
  m_searchInput->setPlaceholderText(tr("Search Company or Customer"));
  m_searchInput->setValidation(AntiquaCRM::ALineEdit::InputValidator::STRINGS);
  addWidget(m_searchInput);

  m_searchBtn = startSearchButton();
  addWidget(m_searchBtn);

  QFrame *m_spacer = new QFrame(this);
  m_spacer->setSizePolicy(QSizePolicy::MinimumExpanding,
                          QSizePolicy::Preferred);
  addWidget(m_spacer);

  const QString _toolTip(tr("Restrict search to current selection."));

  AntiquaCRM::ALabel *m_label = new AntiquaCRM::ALabel(this);
  m_label->setText(tr("Restriction"));
  m_label->setToolTip(_toolTip);
  addWidget(m_label);

  m_datePart = new AntiquaCRM::AComboBox(this);
  m_datePart->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  m_datePart->setToolTip(_toolTip);
  m_datePart->addItem(_icon, m_datePart->withoutDisclosures(), QString());
  m_datePart->addItem(_icon, tr("Since in Year"), "year");
  m_datePart->addItem(_icon, tr("Month in Year"), "month");
  m_datePart->addItem(_icon, tr("Week of Year"), "week");
  m_datePart->addItem(_icon, tr("Day in Month"), "day");
  m_datePart->setCurrentIndex(0);
  addWidget(m_datePart);

  int _y = QDate::currentDate().year();
  m_year = new AntiquaCRM::NumEdit(this);
  m_year->setToolTip(tr("Year"));
  m_year->setRange((_y - 20), _y);
  m_year->setValue(_y);
  addWidget(m_year);

  connect(m_filter, SIGNAL(currentIndexChanged(int)), SLOT(setFilter(int)));
  connect(m_searchInput, SIGNAL(returnPressed()), SLOT(setSearch()));
  connect(m_searchBtn, SIGNAL(clicked()), SLOT(setSearch()));

  m_filter->setCurrentIndex(0);
}

const QString OrdersSearchBar::getDatePart() {
  const QString _year = QString::number(getYear());
  QString _dp = m_datePart->itemData(m_datePart->currentIndex()).toString();
  if (_dp.isEmpty() || _dp == "year") {
    return QString("DATE_PART('year',o_since)=" + _year);
  }

  QString _sql("(");
  _sql.append("DATE_PART('" + _dp + "',o_since)=");
  _sql.append("DATE_PART('" + _dp + "',CURRENT_DATE)");
  // append month to day query
  if (_dp == "day") {
    _sql.append(" AND ");
    _sql.append("DATE_PART('month',o_since)=");
    _sql.append("DATE_PART('month',CURRENT_DATE)");
  }
  // append always year
  _sql.append(" AND DATE_PART('year',o_since)=");
  _sql.append(_year);
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
    m_searchInput->setPlaceholderText(tr("Search Customer or Company"));
    m_searchInput->setValidation(
        AntiquaCRM::ALineEdit::InputValidator::STRINGS);
  } break;

  case 1: {
    m_searchInput->setPlaceholderText(tr("Search Order id"));
    m_searchInput->setValidation(
        AntiquaCRM::ALineEdit::InputValidator::ARTICLE);
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
    m_searchInput->setValidation(m_searchInput->InputValidator::DEFAULT);
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

int OrdersSearchBar::getYear() { return m_year->getValue().toInt(); }

int OrdersSearchBar::searchLength() {
  const QString _search = m_searchInput->text().trimmed();
  return _search.length();
}

const QString OrdersSearchBar::getSearchStatement() {
  int _index = m_filter->currentIndex();
  QString _sql;
  switch (_index) {
  case 0: {
    QStringList sList({"c_company_name", "c_firstname", "c_lastname"});
    QStringList buffer;
    foreach (QString f, sList) {
      buffer << f + " ILIKE '%" + p_search + "%'";
    }
    _sql = "(" + buffer.join(" OR ") + ")";
    _sql.append(" AND " + getDatePart());
  } break;

  case 1:
    _sql = QString("o_id IN (%1)").arg(p_search);
    break;

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
  return _sql;
}
