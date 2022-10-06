// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TABWIDGET_H
#define ANTIQUACRM_TABWIDGET_H

#include <QMainWindow>
#include <QObject>
#include <QTabWidget>
#include <QWidget>

class TabBooks;

class AntiquaTabWidget final : public QTabWidget {
  Q_OBJECT

private:
  TabBooks *m_books;
  QWidget *m_testing;

private Q_SLOTS:
  void tabChanged(int index);

public:
  explicit AntiquaTabWidget(QMainWindow *parent = nullptr);
  bool loadDefaultTabs();
};

#endif // ANTIQUACRM_TABWIDGET_H
