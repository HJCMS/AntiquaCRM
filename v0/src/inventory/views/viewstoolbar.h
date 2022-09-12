// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_VIEWS_TOOLBAR_H
#define INVENTORY_VIEWS_TOOLBAR_H

#include <QComboBox>
#include <QObject>
#include <QToolBar>
#include <QWidget>

#include <SqlCore>

class ViewsToolBar : public QToolBar {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  HJCMS::SqlCore *m_sql;
  QComboBox *m_comboBox;
  QAction *m_query;

  bool initViews();

Q_SIGNALS:
  void s_queryTableView(const QString &tableView);

public Q_SLOTS:
  void refresh();
  void operationChanged(int);

public:
  ViewsToolBar(QWidget *parent = nullptr);
};

#endif // INVENTORY_VIEWS_TOOLBAR_H
