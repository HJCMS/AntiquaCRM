// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_CDSVINYLWIDGET_H
#define ANTIQUACRM_PLUGIN_CDSVINYLWIDGET_H

#include <AntiquaTabs>
#include <QObject>
#include <QScrollArea>
#include <QWidget>

class CDsVinylSearchBar;
class CDsVinylTableView;
class CDsVinylStatusBar;
class CDsVinylEditor;

class ANTIQUACRM_LIBRARY CDsVinylWidget final : public AntiquaCRM::TabsIndex {
  Q_OBJECT

private:
  QWidget *m_mainPage;
  CDsVinylSearchBar *m_searchBar;
  CDsVinylTableView *m_table;
  CDsVinylStatusBar *m_statusBar;

  QScrollArea *m_editorPage;
  CDsVinylEditor *m_editorWidget;

  void popupWarningTabInEditMode() override;

  void setDefaultTableView() override;

public:
  explicit CDsVinylWidget(QWidget *parent = nullptr);

  void openStartPage() override;

  void createSearchQuery(const QString &history = QString()) override;

  void createNewEntry() override;

  void openEntry(qint64 articleId) override;

  void onEnterChanged() override;

  const QString getTitle() const override;

  bool customAction(const QJsonObject &obj) override;
};

#endif // ANTIQUACRM_PLUGIN_CDSVINYLWIDGET_H