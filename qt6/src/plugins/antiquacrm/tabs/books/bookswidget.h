// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_BOOKSWIDGET_H
#define ANTIQUACRM_PLUGIN_BOOKSWIDGET_H

#include <AntiquaTabs>
#include <QScrollArea>
#include <QObject>
#include <QWidget>

class BooksSearchBar;
class BooksTableView;
class BooksStatusBar;
class BooksEditor;

class ANTIQUACRM_LIBRARY BooksWidget final : public AntiquaCRM::TabsIndex {
  Q_OBJECT

private:
  QWidget *m_mainPage;
  BooksSearchBar *m_searchBar;
  BooksTableView *m_table;
  BooksStatusBar *m_statusBar;

  QScrollArea *m_editorPage;
  BooksEditor *m_editorWidget;

  void popupWarningTabInEditMode() override;

  void setDefaultTableView() override;

public:
  explicit BooksWidget(QWidget *parent = nullptr);

  void openStartPage() override;

  void createSearchQuery(const QString &query = QString()) override;

  void createNewEntry() override;

  void openEntry(qint64 articleId) override;

  void onEnterChanged() override;

  AntiquaCRM::TabsIndex::ViewPage currentView() override;

  bool customAction(const QJsonObject &obj) override;
};

#endif // ANTIQUACRM_PLUGIN_BOOKSWIDGET_H