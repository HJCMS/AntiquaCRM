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
  m_selectFilter->setToolTip(
      tr("Press CTRL+Shift+F, to quickly open this Menu."));
  addWidget(m_selectFilter);

  m_searchInput = new AntiquaCRM::ALineEdit(this);
  m_searchInput->setPlaceholderText(tr("Default search"));
  addWidget(m_searchInput);

  addWidget(searchConfines());
  addSeparator();
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
  QString query;
  QString _input = m_searchInput->text();
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
      query.append("(");
      query.append(_bufLeft.join(" OR "));
      query.append(")");
    }
    _bufLeft.clear();
  }

  if (query.length() < 1) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "INVALID_PRINTS_SEARCH_INPUT" << _input;
#endif
    if (!_input.isEmpty())
      return QString("ip_title='" + _input + "'");
    else
      return QString("ip_title='INVALID_PRINTS_SEARCH_INPUT'");
  }
  return query;
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
  if (_data.isEmpty()) {
    qWarning("No filter found.");
    return QString();
  }

  QStringList _fields = _data.split(",");
  QString _sql(withStock() ? "ip_count>0 AND " : "");

  if (_fields.contains("ip_title") || _fields.contains("ip_author")) {
    _sql.append("(" + getSearchString(_fields) + ")");
    return _sql;
  }

  // Article search e.g.(104820,82490,43310)
  static const QRegularExpression numberPattern("[\\D]+");
  if (_fields.contains("ip_id")) {
    QString _str = m_searchInput->text().trimmed();
    _str.replace(numberPattern, ",");
    if (_str.isEmpty())
      return QString();

    _sql.append("ip_id IN (" + _str + ")");
    _str.clear();
    return _sql;
  }

  // Since/Modified Year search
  if (_fields.contains("ip_year")) {
    QStringList _buf;
    QString _str = m_searchInput->text().trimmed();
    _str.replace(numberPattern, ",");
    if (_str.isEmpty())
      return QString();

    _buf << "DATE_PART('YEAR',ip_since) IN (" + _str + ")";
    _buf << "DATE_PART('YEAR',ip_changed) IN (" + _str + ")";
    _sql.append(_buf.join(" OR "));
    _str.clear();
    return _sql;
  }

  // Storage search
  if (_data.contains("ip_storage")) {
    QString _str = m_searchInput->text();
    _str.replace(jokerPattern, "%");
    QStringList _buf;
    _buf << prepareFieldSearch("sl_storage", _str);
    _buf << prepareFieldSearch("ip_storage_compartment", _str);
    _buf << prepareFieldSearch("sl_identifier", _str);
    if (_fields.contains("ip_keyword"))
      _buf << prepareFieldSearch("ip_keyword", _str);

    _sql.append("(" + _buf.join(" OR ") + ")");
    _str.clear();
    return _sql;
  }

  qWarning("Not Defined Search (%s)", qPrintable(_data));
  return QString();
}
