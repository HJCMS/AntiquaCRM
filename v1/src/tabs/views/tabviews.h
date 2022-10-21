// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TABVIEWS_H
#define ANTIQUACRM_TABVIEWS_H

#include <AntiquaWidgets>
#include <QObject>
#include <QSqlTableModel>
#include <QTableView>
#include <QWidget>

class TabViews final : public Inventory {
  Q_OBJECT

private:
  QTableView *m_mainPage;
  QSqlTableModel *m_model;

public Q_SLOTS:
  void openStartPage();
  void createSearchQuery(const QString &query = QString());
  void createNewEntry(){/* disabled */};
  void openEntry(qint64 articleId){/* disabled */};
  void onEnterChanged();

public:
  explicit TabViews(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_TABVIEWS_H
