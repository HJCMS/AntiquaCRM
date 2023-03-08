// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PRINTSSTITCHES_H
#define ANTIQUACRM_PRINTSSTITCHES_H

#include <AntiquaWidgets>
#include <QObject>
#include <QScrollArea>

class PrintsStitchesSearchBar;
class PrintsStitchesTableView;
class PrintsStitchesStatusBar;
class PrintsStitchesEditor;

/**
 * @brief Inventory Prints and Stitches
 * @defgroup AntiquaPrintsStitches Tab Prints and Stitches
 */
class TabPrintsStitches final : public Inventory {
  Q_OBJECT

private:
  QWidget *m_mainPage;
  QScrollArea *m_editorPage;
  PrintsStitchesSearchBar *m_searchBar;
  PrintsStitchesTableView *m_table;
  PrintsStitchesStatusBar *m_statusBar;
  PrintsStitchesEditor *m_editorWidget;

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
  explicit TabPrintsStitches(QWidget *parent = nullptr);
  Inventory::ViewIndex currentView() override;
  bool customAction(const QJsonObject &obj) override;
};

#endif // ANTIQUACRM_PRINTSSTITCHES_H
