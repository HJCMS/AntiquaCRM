// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_STITCHESWIDGET_H
#define ANTIQUACRM_PLUGIN_STITCHESWIDGET_H

#include "stitchesconfig.h"
#include <AntiquaTabs>
#include <QObject>
#include <QScrollArea>
#include <QWidget>

class StitchesSearchBar;
class StitchesTableView;
class StitchesStatusBar;
class StitchesEditor;

class ANTIQUACRM_STITCHES_PLUGIN StitchesWidget final : public AntiquaCRM::TabsIndex {
  Q_OBJECT

private:
  QWidget* m_mainPage;
  StitchesSearchBar* m_searchBar;
  StitchesTableView* m_table;
  StitchesStatusBar* m_statusBar;
  QScrollArea* m_editorPage;
  StitchesEditor* m_editorWidget;
  void setDefaultTableView() override;

public:
  explicit StitchesWidget(QWidget* parent = nullptr);

  void openStartPage() override;

  void createSearchQuery(const QString& history = QString()) override;

  void createNewEntry() override;

  void openEntry(qint64 articleId) override;

  void onEnterChanged() override;

  const QString getTitle() const override;

  bool customAction(const QJsonObject& obj) override;

  const QStringList acceptsCustomActions() const override;
};

#endif // ANTIQUACRM_PLUGIN_STITCHESWIDGET_H
