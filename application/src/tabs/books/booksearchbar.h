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

/**
 * @brief Books search bar
 * @ingroup _books
 */
class BookSearchBar final : public TabSearchBar {
  Q_OBJECT

private:
  BookSelectFilter *m_selectFilter;
  BookSearchLine *m_searchLeft;
  BookSearchLine *m_searchRight;
  QPushButton *m_searchBtn;

  const QString getTitleSearch(const QStringList &fields);

protected Q_SLOTS:
  void setSearch() override;

public Q_SLOTS:
  void setFilter(int index) override;
  void setFilterFocus() override;
  void setClearAndFocus() override;
  void setSearchFocus() override;

public:
  explicit BookSearchBar(QWidget *parent = nullptr);
  int searchLength() override;
  const QString getSearchStatement() override;
};

#endif // ANTIQUACRM_BOOKSEARCHBAR_H
