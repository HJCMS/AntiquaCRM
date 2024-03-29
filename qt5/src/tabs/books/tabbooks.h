// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_TABBOOKS_H
#define ANTIQUACRM_TABBOOKS_H

#include <AntiquaWidgets>
#include <QObject>
#include <QScrollArea>

class BookSearchBar;
class BookTableView;
class BookStatusBar;
class BookEditor;

/**
 * @brief Inventory Books Class
 * @ingroup _books
 */
class TabBooks final : public Inventory {
  Q_OBJECT

private:
  QWidget *m_mainPage;
  BookSearchBar *m_searchBar;
  BookTableView *m_table;
  BookStatusBar *m_statusBar;
  QScrollArea *m_editorPage;
  BookEditor *m_editorWidget;

private Q_SLOTS:
  void popupWarningTabInEditMode() override;
  void setDefaultTableView() override;

public Q_SLOTS:
  void openStartPage() override;
  void createSearchQuery(const QString &query = QString()) override;
  void createNewEntry() override;
  void openEntry(qint64 articleId) override;
  void onEnterChanged() override;

public:
  explicit TabBooks(QWidget *parent = nullptr);
  Inventory::ViewIndex currentView() override;
  bool customAction(const QJsonObject &obj) override;
};

#endif // ANTIQUACRM_TABBOOKS_H
