// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_TABCDVINYL_H
#define ANTIQUACRM_TABCDVINYL_H

#include <AntiquaWidgets>
#include <QObject>
#include <QScrollArea>

class CDVSearchBar;
class CDVTableView;
class CDVStatusBar;
class CDVEditor;

/**
 * @brief Inventory CD+Vinyl Class
 * @ingroup _cdvinyl
 */
class TabCDVinyl final : public Inventory {
  Q_OBJECT

private:
  QWidget *m_mainPage;
  CDVSearchBar *m_searchBar;
  CDVTableView *m_table;
  CDVStatusBar *m_statusBar;
  QScrollArea *m_editorPage;
  CDVEditor *m_editorWidget;

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
  explicit TabCDVinyl(QWidget *parent = nullptr);
  Inventory::ViewIndex currentView() override;
  bool customAction(const QJsonObject &obj) override;
};

#endif // ANTIQUACRM_TABCDVINYL_H
