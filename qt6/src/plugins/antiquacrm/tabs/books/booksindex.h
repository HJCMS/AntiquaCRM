// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_BOOKSINDEX_H
#define ANTIQUACRM_PLUGIN_BOOKSINDEX_H

#include "booksconfig.h"
#include <AntiquaTabs>
#include <QObject>
#include <QScrollArea>
#include <QWidget>

class BooksSearchBar;
class BooksTableView;
class BooksStatusBar;
class BooksEditor;

class ANTIQUACRM_BOOKS_PLUGIN BooksIndex final : public AntiquaCRM::TabsIndex {
  Q_OBJECT

private:
  QWidget* m_mainPage;
  BooksSearchBar* m_searchBar;
  BooksTableView* m_table;
  BooksStatusBar* m_statusBar;
  QScrollArea* m_editorPage;
  BooksEditor* m_editorWidget;
  void setDefaultTableView() override;

public:
  explicit BooksIndex(QWidget* parent = nullptr);
  void openStartPage() override;
  void createSearchQuery(const QString& history = QString()) override;
  void createNewEntry() override;
  void openEntry(qint64 articleId) override;
  void onEnterChanged() override;
  const QString getTitle() const override;
  bool customAction(const QJsonObject& obj) override;
  const QStringList acceptsCustomActions() const override;
};

#endif // ANTIQUACRM_PLUGIN_BOOKSINDEX_H
