// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_VARIOUSINDEX_H
#define ANTIQUACRM_PLUGIN_VARIOUSINDEX_H

#include "variousconfig.h"
#include <AntiquaTabs>
#include <QObject>
#include <QScrollArea>
#include <QWidget>

class VariousSearchBar;
class VariousTableView;
class VariousStatusBar;
class VariousEditor;

class ANTIQUACRM_VARIOUS_PLUGIN VariousIndex final : public AntiquaCRM::TabsIndex {
  Q_OBJECT

private:
  QWidget* m_mainPage;
  VariousSearchBar* m_searchBar;
  VariousTableView* m_table;
  VariousStatusBar* m_statusBar;
  QScrollArea* m_editorPage;
  VariousEditor* m_editorWidget;
  void setDefaultTableView() override;

public:
  explicit VariousIndex(QWidget* parent = nullptr);
  void openStartPage() override;
  void createSearchQuery(const QString& history = QString()) override;
  void createNewEntry() override;
  void openEntry(qint64 articleId) override;
  void onEnterChanged() override;
  const QString getTitle() const override;
  bool customAction(const QJsonObject& obj) override;
  const QStringList acceptsCustomActions() const override;
};

#endif // ANTIQUACRM_PLUGIN_VARIOUSINDEX_H
