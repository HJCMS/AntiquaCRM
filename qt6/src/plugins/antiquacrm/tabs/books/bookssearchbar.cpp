// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookssearchbar.h"
#include "booksselectfilter.h"

#include <QIcon>
#include <QJsonObject>
#include <QRegularExpression>
#include <QToolButton>

BooksSearchBar::BooksSearchBar(QWidget *parent)
    : AntiquaCRM::TabsSearchBar{parent} {
  m_selectFilter = new BooksSelectFilter(this);
  QString filterTip = tr("Press CTRL+Shift+F, to quickly open this Menu.");
  m_selectFilter->setToolTip(filterTip);
  addWidget(m_selectFilter);

  m_searchLeft = new AntiquaCRM::ALineEdit(this);
  addWidget(m_searchLeft);

  QToolButton *m_icontb = new QToolButton(this);
  m_icontb->setEnabled(false);
  m_icontb->setIcon(AntiquaCRM::AntiquaApplIcon("user-group"));
  addWidget(m_icontb);

  m_searchRight = new AntiquaCRM::ALineEdit(this);
  addWidget(m_searchRight);

  addWidget(searchConfines());
  addSeparator();
  addWidget(stockCheckBox());

  m_searchBtn = startSearchButton();
  addWidget(m_searchBtn);

  connect(m_selectFilter, SIGNAL(currentIndexChanged(int)),
          SLOT(setFilter(int)));
  connect(m_searchLeft, SIGNAL(returnPressed()), SLOT(setSearch()));
  connect(m_searchRight, SIGNAL(returnPressed()), SLOT(setSearch()));
  connect(m_searchBtn, SIGNAL(clicked()), SLOT(setSearch()));
}

const QString BooksSearchBar::getTitleSearch(const QStringList &fields) {
  QString query;
  QString inputLeft = m_searchLeft->text();
  QString inputRight = m_searchRight->text();
  // Standard Suchfeld
  if (m_searchLeft->length() >= getMinLength()) {
    QStringList bufferLeft;
    foreach (QString f, fields) {
      if (f != "ib_author") {
        QString fset = prepareFieldSearch(f, inputLeft);
        if (fset.isEmpty())
          continue;

        bufferLeft << fset;
      }
    }
    if (bufferLeft.count() > 0) {
      query.append("(");
      query.append(bufferLeft.join(" OR "));
      query.append(")");
    }
    bufferLeft.clear();
  }
  // Autoren Suchfeld
  if (m_searchRight->isEnabled() && m_searchRight->length() >= getMinLength()) {
    QStringList bufferRight;
    if (m_searchRight->placeholderText().contains(tr("Keyword")))
      bufferRight << prepareFieldSearch("ib_keyword", inputRight);
    else
      bufferRight << prepareFieldSearch("ib_author", inputRight);

    if (query.isEmpty())
      query.append("(");
    else
      query.append(" AND (");

    query.append(bufferRight.join(" OR "));
    query.append(")");
    bufferRight.clear();
  }
  if (query.length() < 1) {
    // NOTE prevent empty where clauses
    qWarning("INVALID_TITLE_SEARCH");
    if (!inputLeft.isEmpty())
      return QString("ib_title='" + inputLeft + "'");
    else if (!inputRight.isEmpty())
      return QString("ib_title='" + inputRight + "'");
    else
      return QString("ib_title='INVALID_TITLE_SEARCH'");
  }
  return query;
}

void BooksSearchBar::setSearch() {
  if (m_searchLeft->isEnabled() && searchLength() >= getMinLength()) {
    emit sendSearchClicked();
  } else if (m_searchRight->isEnabled() && searchLength() >= getMinLength()) {
    emit sendSearchClicked();
  } else
    emit sendNotify(tr("Your input is too short, increase your search!"));
}

void BooksSearchBar::setFilter(int index) {
  QVariant val = m_selectFilter->itemData(index, Qt::UserRole);
  BooksSelectFilter::Filter f = qvariant_cast<BooksSelectFilter::Filter>(val);
  m_searchLeft->setEnabled(true);
  m_searchLeft->clear();
  m_searchRight->clear();

  QString leftTr = tr("Search Booktitle");
  QString rightTr = tr("Authors search");

  switch (f) {
  case (BooksSelectFilter::ArticleId): {
    m_searchLeft->setValidation(AntiquaCRM::ALineEdit::InputValidator::ARTICLE);
    m_searchLeft->setPlaceholderText(
        tr("Single Article number or multiple separated by comma."));
    m_searchRight->setEnabled(false);
    m_searchRight->setPlaceholderText(rightTr);
    break;
  }

  case (BooksSelectFilter::ISBN): {
    m_searchLeft->setValidation(AntiquaCRM::ALineEdit::InputValidator::NUMERIC);
    m_searchLeft->setPlaceholderText(tr("Search by ISBN number"));
    m_searchRight->setEnabled(false);
    m_searchRight->setPlaceholderText(rightTr);
    break;
  }

  case (BooksSelectFilter::BooksAuthor): {
    m_searchLeft->setValidation(AntiquaCRM::ALineEdit::InputValidator::STRINGS);
    m_searchLeft->setPlaceholderText(leftTr);
    m_searchRight->setValidation(
        AntiquaCRM::ALineEdit::InputValidator::STRINGS);
    m_searchRight->setEnabled(true);
    m_searchRight->setPlaceholderText(tr("and Authors"));
    break;
  }

  case (BooksSelectFilter::BooksKeyword): {
    m_searchLeft->setValidation(AntiquaCRM::ALineEdit::InputValidator::STRINGS);
    m_searchLeft->setPlaceholderText(leftTr);
    m_searchRight->setValidation(
        AntiquaCRM::ALineEdit::InputValidator::STRINGS);
    m_searchRight->setEnabled(true);
    m_searchRight->setPlaceholderText(tr("Keyword"));
    break;
  }

  case (BooksSelectFilter::Storage): {
    m_searchLeft->setValidation(AntiquaCRM::ALineEdit::InputValidator::STRINGS);
    m_searchLeft->setPlaceholderText(tr("Storage"));
    m_searchRight->setEnabled(false);
    break;
  }

  case (BooksSelectFilter::Authors): {
    m_searchLeft->setValidation(AntiquaCRM::ALineEdit::InputValidator::STRINGS);
    m_searchLeft->setEnabled(false);
    m_searchLeft->setPlaceholderText(leftTr);
    m_searchRight->setValidation(
        AntiquaCRM::ALineEdit::InputValidator::STRINGS);
    m_searchRight->setEnabled(true);
    m_searchRight->setPlaceholderText(rightTr);
    break;
  }

  default: {
    m_searchLeft->setValidation(AntiquaCRM::ALineEdit::InputValidator::STRINGS);
    m_searchRight->setEnabled(false);
    m_searchLeft->setPlaceholderText(leftTr);
    m_searchRight->setPlaceholderText(rightTr);
    break;
  }
  };

  emit sendFilterChanged(index);
}

void BooksSearchBar::setFilterFocus() {
  m_selectFilter->setFocus();
  m_selectFilter->showPopup();
}

void BooksSearchBar::setClearAndFocus() {
  m_searchLeft->clear();
  m_searchRight->clear();
  m_searchLeft->setFocus();
}

void BooksSearchBar::setSearchFocus() { setClearAndFocus(); }

int BooksSearchBar::searchLength() {
  return (m_searchLeft->length() + m_searchRight->length());
}

const QString BooksSearchBar::getSearchStatement() {
  QJsonObject js = m_selectFilter->getFilter(m_selectFilter->currentIndex());
  QStringList fields = js.value("fields").toString().split(",");
  QString sql(withStock() ? "ib_count>0 AND " : "");
  QString operation = js.value("search").toString();
  if (operation.isEmpty())
    return QString();

  // Title und Autorensuche
  if (operation == "title_and_author") {
    sql.append("(" + getTitleSearch(fields) + ")");
    return sql;
  }

  // Buch Titlesuche
  if (operation == "title") {
    sql.append("(" + getTitleSearch(fields) + ")");
    return sql;
  }

  // Buch Autorensuche
  if (operation == "author") {
    sql.append("(" + getTitleSearch(fields) + ")");
    return sql;
  }

  // Artikel Nummersuche (107368,115110)
  if (operation == "articleId") {
    QString s = m_searchLeft->text();
    s.replace(QRegularExpression("\\,\\s?$"), "");
    return "ib_id IN (" + s + ")";
  }

  // ISBN Suche
  if (operation == "isbn") {
    QString s = m_searchLeft->text();
    s.replace(QRegularExpression("\\D+"), "");
    if (s.length() == 10 || s.length() == 13) {
      sql.append("ib_isbn=" + s);
      return sql;
    }
  }

  // Publisher
  if (operation == "publisher") {
    QString s = m_searchLeft->text();
    sql.append("(" + prepareFieldSearch("ib_publisher", s) + ")");
    return sql;
  }

  // Lager & Stichwortsuche
  if (operation == "storage") {
    QString s = m_searchLeft->text();
    s.replace(jokerPattern, "%");
    QStringList buffer;
    buffer << prepareFieldSearch("sl_storage", s);
    buffer << prepareFieldSearch("sl_identifier", s);
    buffer << prepareFieldSearch("ib_keyword", s);
    sql.append("(" + buffer.join(" OR ") + ")");
    return sql;
  }

  qWarning("Not Defined Search (%s)",
           qPrintable(js.value("search").toString()));

  return QString();
}
