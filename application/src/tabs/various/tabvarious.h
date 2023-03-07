// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TABVARIOUS_H
#define ANTIQUACRM_TABVARIOUS_H

#include <AntiquaWidgets>
#include <QObject>
#include <QScrollArea>

class VariousSearchBar;
class VariousTableView;
class VariousEditor;
class VariousStatusBar;

/**
 * @brief Inventory Various
 * @section InventoryVarious
 */
class TabVarious final : public Inventory {
  Q_OBJECT

private:
  QWidget *m_mainPage;
  VariousSearchBar *m_searchBar;
  VariousTableView *m_table;
  VariousEditor *m_editorWidget;
  VariousStatusBar *m_statusBar;
  QScrollArea *m_editorPage;

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
  explicit TabVarious(QWidget *parent = nullptr);
  Inventory::ViewIndex currentView() override;
  bool customAction(const QJsonObject &obj) override;
};

#endif // ANTIQUACRM_TABVARIOUS_H
