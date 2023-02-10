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

class BookSearchBar final : public TabSearchBar {
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
  bool withStock();
};

#endif // ANTIQUACRM_BOOKSEARCHBAR_H
