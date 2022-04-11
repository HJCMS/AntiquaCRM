// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DOCKBARWIDGET_H
#define DOCKBARWIDGET_H

#include "applsettings.h"

/* QtCore */
#include <QtCore/QEvent>
#include <QtCore/QObject>

/* QtWidgets */
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

class DockBarWidget : public QDockWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  ApplSettings *m_cfg;
  QTabWidget *m_tabWidget;

private Q_SLOTS:
  void dockPositionChanged(Qt::DockWidgetArea);
  void tabHideShowAction(int index);

public:
  DockBarWidget(QWidget *parent = nullptr);
};

#endif // DOCKBARWIDGET_H
