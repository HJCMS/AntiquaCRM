// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "stitchessearchbar.h"
#include "stitchesselectfilter.h"

#include <QIcon>
#include <QJsonObject>
#include <QRegularExpression>
#include <QToolButton>

StitchesSearchBar::StitchesSearchBar(QWidget *parent)
    : AntiquaCRM::TabsSearchBar{parent} {
  m_selectFilter = new StitchesSelectFilter(this);
  addWidget(m_selectFilter);

  m_searchInput = new AntiquaCRM::ALineEdit(this);
  m_searchInput->setPlaceholderText(tr("Default search"));
  addWidget(m_searchInput);

  // ip_landscape
  m_orientation = new QComboBox(this);
  m_orientation->insertItem(0, tr("Orientation"));
  m_orientation->insertItem(1, tr("Vertical"));
  m_orientation->insertItem(2, tr("Horizontal"));
  m_orientation->setToolTip(tr("Orientation: Any, Portrait or Landscape"));
  addWidget(m_orientation);

  // ip_views
  m_views = new QCheckBox(tr("Views"), this);
  m_views->setToolTip(tr("With Views or not."));
  addWidget(m_views);

  addWidget(stockCheckBox());

  m_searchBtn = startSearchButton();
  addWidget(m_searchBtn);

  connect(m_selectFilter, SIGNAL(currentIndexChanged(int)),
          SLOT(setFilter(int)));

  connect(m_searchInput, SIGNAL(returnPressed()), SLOT(setSearch()));
  connect(m_searchBtn, SIGNAL(clicked()), SLOT(setSearch()));

  setFilter(0);
}

const QString StitchesSearchBar::getSearchString(const QStringList &fields) {
  QString _sql;
  QString _input = m_searchInput->text().trimmed();
  if (_input.isEmpty() || fields.count() < 1) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "INVALID_PRINTS_SEARCH_INPUT";
#endif
    return QString("ip_title='INVALID_PRINTS_SEARCH_INPUT'");
  }

  // Standard Suchfeld
  if (_input.length() >= getMinLength()) {
    QStringList _bufLeft;
    foreach (QString f, fields) {
      QString fset = prepareFieldSearch(f, _input);
      if (fset.isEmpty())
        continue;

      _bufLeft << fset;
    }
    if (_bufLeft.count() > 0) {
      _sql.append("(");
      _sql.append(_bufLeft.join(" OR "));
      _sql.append(")");
    }
    _bufLeft.clear();
  }

  return _sql;
}

bool StitchesSearchBar::lineInputsEnabled() {
  return m_searchInput->isEnabled();
}

void StitchesSearchBar::setSearch() {
  QString _filter = m_selectFilter->currentFilter();
  if (_filter.isEmpty())
    return;

  if (_filter.contains("ip_id")) {
    emit sendSearchClicked();
  } else if (lineInputsEnabled() && requiredLengthExists()) {
    emit sendSearchClicked();
  } else {
    emit sendNotify(tr("Your input is too short, increase your search!"));
  }
}

void StitchesSearchBar::setFilter(int index) {
  m_searchInput->clear();
  m_searchInput->setEnabled(true);
  m_searchInput->setToolTip(QString());

  const QString _tip = m_selectFilter->currentToolTip(index);
  const QString _filter = m_selectFilter->currentFilter(index);

  if (_filter.contains("ip_id") || _filter.contains("ip_since")) {
    m_searchInput->setValidation(
        AntiquaCRM::ALineEdit::InputValidator::ARTICLE);
  } else {
    m_searchInput->setValidation(
        AntiquaCRM::ALineEdit::InputValidator::STRINGS);
  }
  m_searchInput->setToolTip(_tip);
  m_searchInput->setPlaceholderText(_tip);

  emit sendFilterChanged(index);
}

void StitchesSearchBar::setFilterFocus() {
  m_selectFilter->setFocus();
  m_selectFilter->showPopup();
}

void StitchesSearchBar::setClearAndFocus() {
  m_searchInput->clear();
  m_searchInput->setFocus();
}

void StitchesSearchBar::setSearchFocus() { setClearAndFocus(); }

int StitchesSearchBar::searchLength() { return m_searchInput->length(); }

bool StitchesSearchBar::requiredLengthExists() {
  return (searchLength() >= getMinLength());
}

const QString StitchesSearchBar::getSearchStatement() {
  QString _data = m_selectFilter->currentFilter();
  QString _input = m_searchInput->text().trimmed();
  if (_data.isEmpty() || _input.isEmpty()) {
    qWarning("No filter found.");
    emit sendNotify(tr("Invalid search input!"));
    return QString();
  }

  // start SqlClause with or without stock
  QString _sql(withStock() ? "ip_count>0 AND " : "");

  // ip_views
  if (m_views->isChecked())
    _sql.append("ip_views=true AND ");

  // ip_landscape
  if (m_orientation->currentIndex() == 2)
    _sql.append("ip_landscape=true AND ");
  else if (m_orientation->currentIndex() == 1)
    _sql.append("ip_landscape=false AND ");

  // table columns from filter
  QStringList _cols = _data.split(",");

  // default string search patterns
  if (_cols.contains("ip_title") || _cols.contains("ip_author")) {
    _sql.append(getSearchString(_cols));
    return _sql;
  }

  // article search e.g.(104820,82490,43310)
  static const QRegularExpression numberPattern("[\\D]+");
  if (_cols.contains("ip_id")) {
    QString _str = _input;
    _str.replace(numberPattern, ",");
    if (_str.isEmpty())
      return QString();

    _sql.append("ip_id IN (" + _str + ")");
    _str.clear();
    return _sql;
  }

  // year search - since and modified
  if (_cols.contains("ip_year")) {
    QStringList _buf;
    QString _str = _input;
    _str.replace(numberPattern, ",");
    if (_str.isEmpty())
      return QString();

    _buf << "DATE_PART('YEAR',ip_since) IN (" + _str + ")";
    _buf << "DATE_PART('YEAR',ip_changed) IN (" + _str + ")";
    _sql.append("(" + _buf.join(" OR ") + ")");
    _str.clear();
    return _sql;
  }

  // storage and keyword search
  if (_data.contains("ip_storage")) {
    QString _str = _input;
    _str.replace(jokerPattern, "%");

    QStringList _buf;
    _buf << prepareFieldSearch("sl_storage", _str);
    _buf << prepareFieldSearch("ip_storage_compartment", _str);
    _buf << prepareFieldSearch("sl_identifier", _str);
    if (_cols.contains("ip_keyword"))
      _buf << prepareFieldSearch("ip_keyword", _str);

    _sql.append("(" + _buf.join(" OR ") + ")");
    _str.clear();
    return _sql;
  }

  qWarning("No defined Search (%s)", qPrintable(_data));
  return QString();
}
