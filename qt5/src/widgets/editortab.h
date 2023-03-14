// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_EDITORTAB_H
#define ANTIQUACRM_EDITORTAB_H

#include <QObject>
#include <QTabBar>
#include <QTabWidget>
#include <QWheelEvent>
#include <QIcon>

/**
 * @brief The EditorTabBar class
 * @ingroup widgets
 */
class EditorTabBar final : public QTabBar {
  Q_OBJECT

protected:
  void wheelEvent(QWheelEvent *){/* disabled */};

public:
  explicit EditorTabBar(QWidget *parent = nullptr) : QTabBar{parent} {};
};

/**
 * @brief The EditorTab Widget
 * @ingroup widgets
 */
class EditorTab : public QTabWidget {
  Q_OBJECT

private:
  EditorTabBar *m_tabBar;

public:
  explicit EditorTab(QWidget *parent = nullptr);
  static const QIcon defaultIcon();
};

#endif // ANTIQUACRM_EDITORTAB_H
