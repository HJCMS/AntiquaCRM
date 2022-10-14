// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_BOOKSEARCHBAR_H
#define ANTIQUACRM_BOOKSEARCHBAR_H

#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>

class BookSelectFilter;
class BookSearchLine;

class BookSearchBar final : public SearchBar {
  Q_OBJECT

private:
  int minLength = 2;
  QString p_currentLeft;
  QString p_currentRight;
  BookSelectFilter *m_selectFilter;
  BookSearchLine *m_searchLeft;
  BookSearchLine *m_searchRight;
  QPushButton *m_searchBtn;

  const QString prepareFieldSet(const QString &fieldname,
                                const QString &search) const;

  const QString getTitleSearch(const QStringList &fields);

protected Q_SLOTS:
  void setSearch();

public Q_SLOTS:
  void setFilter(int index);
  void setFilterFocus();
  void setClearAndFocus();
  void setSearchFocus();

public:
  explicit BookSearchBar(QWidget *parent = nullptr);
  int searchLength();
  const QString getSearchStatement();
};

#endif // ANTIQUACRM_BOOKSEARCHBAR_H
