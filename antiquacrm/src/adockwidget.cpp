#include "version.h"
#include "adockwidget.h"

#include <QtCore/QDebug>
#include <QtCore/QSize>

ADockWidget::ADockWidget(QWidget * parent)
    : QDockWidget{parent}
{
    setObjectName("ExtendetSearchWidget");
    setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    setFeatures(QDockWidget::DockWidgetMovable);

    m_cfg = new ApplSettings();
    m_tabWidget = new QTabWidget(this);

    QWidget* test = new QWidget(m_tabWidget);
    m_tabWidget->addTab(test,myIcon("search"),tr("Test Tab"));

    setWidget(m_tabWidget);

    connect(this, SIGNAL ( dockLocationChanged(Qt::DockWidgetArea) ),
            this, SLOT ( dockPositionChanged(Qt::DockWidgetArea)) );

    connect(m_tabWidget, SIGNAL ( tabBarDoubleClicked(int) ),
            this, SLOT ( tabHideShowAction(int)) );
}

/**
 * @brief ADockWidget::dockPositionChanged
 * @short Wir verwenden das Andockfenster nur links und rechts.
 *        Tab Position je nach Seite ändern!
 * @param area
 */
void ADockWidget::dockPositionChanged(Qt::DockWidgetArea area)
{
    if(area == Qt::LeftDockWidgetArea)
    {
        m_tabWidget->setTabPosition(QTabWidget::West);
        m_cfg->setValue("dockarea/position","left");
    }
    else if(area == Qt::RightDockWidgetArea)
    {
        m_tabWidget->setTabPosition(QTabWidget::East);
        m_cfg->setValue("dockarea/position","right");
    }
}

/**
 * @brief ADockWidget::tabHideShowAction
 * @todo Swap IN/OUT Function
 */
void ADockWidget::tabHideShowAction(int)
{}
