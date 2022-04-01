// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@


#ifndef ADOCKWIDGET_H
#define ADOCKWIDGET_H

#include "applsettings.h"

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QEvent>

/* QtWidgets */
#include <QtWidgets/QWidget>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QTabWidget>

class ADockWidget : public QDockWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

private:
    ApplSettings* m_cfg;
    QTabWidget* m_tabWidget;

private Q_SLOTS:
    void dockPositionChanged(Qt::DockWidgetArea);
    void tabHideShowAction(int);

public:
    ADockWidget(QWidget* parent = 0);

};

#endif // EXTENDETSEARCH_H
