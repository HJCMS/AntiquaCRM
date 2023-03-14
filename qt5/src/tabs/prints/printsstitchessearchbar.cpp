// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printsstitchessearchbar.h"
#include "printsstitchesfilterselect.h"
#include "printsstitchessearchline.h"

#include <QIcon>
#include <QToolButton>

PrintsStitchesSearchBar::PrintsStitchesSearchBar(QWidget *parent)
    : TabSearchBar{parent} {
  setObjectName("printsstitches_search_bar");

  m_selectFilter = new PrintsStitchesFilterSelect(this);
  QString filterTip = tr("Press CTRL+Shift+F, to quickly open this Menu.");
  m_selectFilter->setToolTip(filterTip);
  addWidget(m_selectFilter);

  m_searchEdit = new PrintsStitchesSearchLine(this);
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

void PrintsStitchesSearchBar::setSearch() {
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

void PrintsStitchesSearchBar::setFilter(int index) {
  QVariant val = m_selectFilter->itemData(index, Qt::UserRole);
  PrintsStitchesFilterSelect::PSFilter filter =
      static_cast<PrintsStitchesFilterSelect::PSFilter>(val.toInt());

  QString se(tr("Search") + " ");

  switch (filter) {
  case PrintsStitchesFilterSelect::PSFilter::PSF_TITLE:
    m_searchEdit->setValidation(SearchLineEdit::Strings);
    m_searchEdit->setPlaceholderText(se + tr("Title"));
    break;

  case PrintsStitchesFilterSelect::PSFilter::PSF_KEYWORDS:
    m_searchEdit->setValidation(SearchLineEdit::Strings);
    m_searchEdit->setPlaceholderText(se + tr("Keyword"));
    break;

  case PrintsStitchesFilterSelect::PSFilter::PSF_AUTHOR:
    m_searchEdit->setValidation(SearchLineEdit::Strings);
    m_searchEdit->setPlaceholderText(se + tr("Author"));
    break;

  case PrintsStitchesFilterSelect::PSFilter::PSF_ARTICLE:
    m_searchEdit->setValidation(SearchLineEdit::Article);
    m_searchEdit->setPlaceholderText(se + tr("Article number"));
    break;

  default:
    m_searchEdit->setValidation(SearchLineEdit::Strings);
    m_searchEdit->setPlaceholderText(se + tr("Title"));
    break;
  };

  setClearAndFocus();
  emit sendFilterChanged(index);
}

void PrintsStitchesSearchBar::setFilterFocus() {
  m_selectFilter->setFocus();
  m_selectFilter->showPopup();
}

void PrintsStitchesSearchBar::setClearAndFocus() {
  m_searchEdit->clear();
  m_searchEdit->setFocus();
}

void PrintsStitchesSearchBar::setSearchFocus() { setClearAndFocus(); }

int PrintsStitchesSearchBar::searchLength() {
  return m_searchEdit->getLength();
}

const QString PrintsStitchesSearchBar::getSearchStatement() {
  if (!m_searchEdit->isValid(getMinLength()))
    return QString();

  QJsonObject js = m_selectFilter->getFilter(m_selectFilter->currentIndex());
  QString search = m_searchEdit->getSearch();
  QString operation = js.value("search").toString();
  QString sql(withStock() ? "ip_count>0 AND " : "");

  // Artikel Nummernsuche (107368,115110)
  if (operation == "articleId") {
    search.replace(QRegExp("\\,\\s?$"), "");
    return "ip_id IN (" + search + ")";
  }

  // Lager
  if (operation == "storage") {
    search.replace(jokerPattern, "%");
    sql.append("(sl_storage ILIKE '");
    sql.append(search + "' OR sl_identifier ILIKE '" + search + "%')");
    sql.append(" OR (ip_keyword ILIKE '" + search + "%')");
    return sql;
  }

  // Titlesuche
  if (operation == "title") {
    QStringList s_pattern;
    s_pattern << prepareFieldSearch("ip_title", search);
    s_pattern << prepareFieldSearch("ip_title_extended", search);
    sql.append("(" + s_pattern.join(" OR ") + ")");
    return sql;
  }

  // Autorensuche
  if (operation == "author") {
    QStringList s_pattern;
    s_pattern << prepareFieldSearch("ip_author", search);
    s_pattern << prepareFieldSearch("ip_description", search);
    sql.append("(" + s_pattern.join(" OR ") + ")");
    return sql;
  }

  // Schlüsselwort suche
  if (operation == "keywords") {
    QStringList buffer;
    QStringList fields = js.value("fields").toString().split(",");
    foreach (QString f, fields) {
      buffer.append(prepareFieldSearch(f, search));
    }
    sql.append("(" + buffer.join(" OR ") + ")");
    return sql;
  }

  qWarning("Not Defined Search (%s)",
           qPrintable(js.value("search").toString()));

  return QString();
}
