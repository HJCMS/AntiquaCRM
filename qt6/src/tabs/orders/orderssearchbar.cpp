// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderssearchbar.h"

#include <QDate>
#include <QFrame>
#include <QToolButton>

OrdersSearchBar::OrdersSearchBar(QWidget *parent)
    : AntiquaCRM::TabsSearchBar{parent}, cDate{QDate::currentDate()} {

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
  m_datePart->addItem(_icon, tr("With year"), "year");
  m_datePart->addItem(_icon, tr("Current month"), "month");
  m_datePart->addItem(_icon, tr("Current Week"), "week");
  m_datePart->setCurrentIndex(0);
  addWidget(m_datePart);

  m_year = new AntiquaCRM::NumEdit(this);
  m_year->setToolTip(tr("Year"));
  m_year->setRange((cDate.year() - 20), cDate.year());
  m_year->setValue(cDate.year());
  addWidget(m_year);

  connect(m_filter, SIGNAL(currentIndexChanged(int)), SLOT(setFilter(int)));
  connect(m_searchInput, SIGNAL(returnPressed()), SLOT(setSearch()));
  connect(m_searchBtn, SIGNAL(clicked()), SLOT(setSearch()));

  m_filter->setCurrentIndex(0);
  min_length = getMinLength();
}

void OrdersSearchBar::setSearch() {
  QString _search = m_searchInput->text().trimmed();
  if (_search.length() < min_length) {
    QString _m = tr("Query length is smaller than %1.").arg(min_length);
    emit sendNotify(_m);
    setSearchFocus();
    return;
  }

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
    min_length = getMinLength();
  } break;

  case 1: {
    m_searchInput->setPlaceholderText(tr("Search Order id"));
    m_searchInput->setValidation(
        AntiquaCRM::ALineEdit::InputValidator::ARTICLE);
    min_length = 1;
  } break;

  case 2: {
    m_searchInput->setPlaceholderText(tr("Search Delivery Service"));
    m_searchInput->setValidation(
        AntiquaCRM::ALineEdit::InputValidator::STRINGS);
    min_length = 3;
  } break;

  case 3: {
    m_searchInput->setPlaceholderText(tr("Search Provider"));
    m_searchInput->setValidation(
        AntiquaCRM::ALineEdit::InputValidator::STRINGS);
    min_length = getMinLength();
  } break;

  default:
    m_searchInput->setPlaceholderText(tr("Search ..."));
    m_searchInput->setValidation(m_searchInput->InputValidator::DEFAULT);
    min_length = getMinLength();
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

const QString OrdersSearchBar::past12Months() const {
  QString _sql("(o_since BETWEEN ");
  _sql.append("(CURRENT_TIMESTAMP - justify_interval(interval '12 months'))");
  _sql.append(" AND CURRENT_TIMESTAMP)");
  return _sql;
}

int OrdersSearchBar::getYear() {
  if (m_datePart->currentIndex() == 0)
    return cDate.year();

  return m_year->getValue().toInt();
}

const QString OrdersSearchBar::getDatePart() {
  int _index = m_datePart->currentIndex();
  // Kein index, dann die letzen 12 Monate!
  if (_index == 0)
    return QString();

  // Starte optionale abfragen
  const QString _part = m_datePart->itemData(_index).toString();
  if (_part.startsWith("year"))
    return QString(" AND DATE_PART('year',o_since)=" +
                   QString::number(getYear()));

  const QDate _date(getYear(), cDate.month(), cDate.day());
  const QString _date_str = _date.toString("yyyy-MM-dd");
  QString _sql("DATE_PART('" + _part + "',o_since)=");
  _sql.append("DATE_PART('" + _part + "',DATE('" + _date_str + "'))");
  if (_index != 0)
    _sql.append(" AND DATE_PART('year',o_since)=" + QString::number(getYear()));

  return _sql;
}

int OrdersSearchBar::searchLength() {
  const QString _search = m_searchInput->text().trimmed();
  return _search.length();
}

const QString OrdersSearchBar::getSearchStatement() {
  int _index = m_filter->currentIndex();
  QString _sql;
  if (p_search.length() < 1)
    return QString("o_delivered IS NULL AND o_order_status<4");

  switch (_index) {
  case 0: {
    QStringList sList({"c_company_name", "c_firstname", "c_lastname"});
    QStringList buffer;
    foreach (QString f, sList) {
      buffer << f + " ILIKE '%" + p_search + "%'";
    }
    _sql = "(" + buffer.join(" OR ") + ")";
    _sql.append(getDatePart());
  } break;

  case 1:
    _sql = QString("o_id IN (%1)").arg(p_search);
    break;

  case 2:
    _sql = QString("d_name ILIKE '%1%'").arg(p_search);
    _sql.append(getDatePart());
    break;

  case 3:
    _sql = QString("o_provider_name ILIKE '%1%'").arg(p_search);
    _sql.append(getDatePart());
    break;

  default:
    _sql.clear();
    break;
  }

  _sql.replace("%%", "%");
  return _sql;
}
