// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_EDITORTAB_H
#define ANTIQUACRM_EDITORTAB_H

#include <QObject>
#include <QTabBar>
#include <QTabWidget>
#include <QWheelEvent>
#include <QIcon>

class EditorTabBar final : public QTabBar {
  Q_OBJECT

protected:
  void wheelEvent(QWheelEvent *){/* disabled */};

public:
  explicit EditorTabBar(QWidget *parent = nullptr) : QTabBar{parent} {};
};

class EditorTab : public QTabWidget {
  Q_OBJECT

private:
  EditorTabBar *m_tabBar;

public:
  explicit EditorTab(QWidget *parent = nullptr);
  static const QIcon defaultIcon();
};

#endif // ANTIQUACRM_EDITORTAB_H
