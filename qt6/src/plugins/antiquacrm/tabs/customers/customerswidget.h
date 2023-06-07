// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_CUSTOMERSWIDGET_H
#define ANTIQUACRM_PLUGIN_CUSTOMERSWIDGET_H

#include <AntiquaTabs>
#include <QObject>
#include <QScrollArea>
#include <QWidget>

class CustomersSearchBar;
class CustomersTableView;
class CustomersStatusBar;
class CustomersEditor;

class ANTIQUACRM_LIBRARY CustomersWidget final : public AntiquaCRM::TabsIndex {
  Q_OBJECT

private:
  QWidget *m_mainPage;
  CustomersSearchBar *m_searchBar;
  CustomersTableView *m_table;
  CustomersStatusBar *m_statusBar;

  QScrollArea *m_editorPage;
  CustomersEditor *m_editorWidget;

  void popupWarningTabInEditMode() override;

  void setDefaultTableView() override;

public:
  explicit CustomersWidget(QWidget *parent = nullptr);

  void openStartPage() override;

  void createSearchQuery(const QString &history = QString()) override;

  void createNewEntry() override;

  void openEntry(qint64 articleId) override;

  void onEnterChanged() override;

  const QString getTitle() const override;

  AntiquaCRM::TabsIndex::ViewPage currentView() override;

  bool customAction(const QJsonObject &obj) override;
};

#endif // ANTIQUACRM_PLUGIN_CustomersWIDGET_H
