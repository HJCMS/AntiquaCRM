// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DOCKBARWIDGET_H
#define DOCKBARWIDGET_H

#include "applsettings.h"

/* QtCore */
#include <QEvent>
#include <QObject>

/* QtWidgets */
#include <QDockWidget>
#include <QTabWidget>
#include <QWidget>

class DockBarWidget : public QDockWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  ApplSettings *m_cfg;
  QTabWidget *m_tabWidget;
  QWidget *m_search;

private Q_SLOTS:
  void dockPositionChanged(Qt::DockWidgetArea);
  void tabHideShowAction(int index);

public:
  DockBarWidget(QWidget *parent = nullptr);
};

#endif // DOCKBARWIDGET_H
