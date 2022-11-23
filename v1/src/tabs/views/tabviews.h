// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TABVIEWS_H
#define ANTIQUACRM_TABVIEWS_H

#include <AntiquaWidgets>
#include <QLabel>
#include <QObject>
#include <QSqlTableModel>
#include <QTableView>
#include <QWidget>

class ViewsTableModel;

class TabViews final : public Inventory {
  Q_OBJECT

private:
  QLabel *m_label;
  QTableView *m_tableView;
  ViewsTableModel *m_model;

  void popupWarningTabInEditMode() override {};

public Q_SLOTS:
  void openStartPage() override;
  void createSearchQuery(const QString &query = QString()) override;
  void createNewEntry() override {/* disabled */};
  void openEntry(qint64) override {/* disabled */};
  void onEnterChanged() override;

public:
  explicit TabViews(QWidget *parent = nullptr);
  bool customAction(const QJsonObject &) override { return false; }
};

#endif // ANTIQUACRM_TABVIEWS_H
