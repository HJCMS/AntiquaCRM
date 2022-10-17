// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TABWIDGET_H
#define ANTIQUACRM_TABWIDGET_H

#include <AntiquaWidgets>
#include <QMainWindow>
#include <QObject>
#include <QTabWidget>
#include <QWidget>

class AntiquaTabBar;
class TabBooks;

class AntiquaTabWidget final : public QTabWidget {
  Q_OBJECT

private:
  AntiquaTabBar *m_tabBar;
  TabBooks *m_books;
  QWidget *m_testing;
  inline Inventory *tabWidget(int index) const;

private Q_SLOTS:
  void setTabChanged(int index);
  void setTabToClose(int index);
  void setTabToVisit(int index);

public:
  explicit AntiquaTabWidget(QMainWindow *parent = nullptr);
  bool loadDefaultTabs();
};

#endif // ANTIQUACRM_TABWIDGET_H
