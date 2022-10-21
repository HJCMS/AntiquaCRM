// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TABBOOKS_H
#define ANTIQUACRM_TABBOOKS_H

#include <AntiquaWidgets>
#include <QObject>
#include <QScrollArea>

class BookSearchBar;
class BookTableView;
class BookStatusBar;
class BookEditor;

class TabBooks final : public Inventory {
  Q_OBJECT

private:
  QWidget *m_mainPage;
  BookSearchBar *m_searchBar;
  BookTableView *m_table;
  BookStatusBar *m_statusBar;
  QScrollArea *m_editorPage;
  BookEditor *m_editorWidget;

public Q_SLOTS:
  void openStartPage();
  void createSearchQuery(const QString &query = QString());
  void createNewEntry();
  void openEntry(qint64 articleId);
  void onEnterChanged();

public:
  explicit TabBooks(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_TABBOOKS_H
