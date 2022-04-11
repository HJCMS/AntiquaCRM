#include "dockbarwidget.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtCore/QSize>
#include <QtWidgets/QTabBar>

DockBarWidget::DockBarWidget(QWidget *parent) : QDockWidget{parent} {
  setObjectName("DockAreaWidget");
  setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  setFeatures(QDockWidget::DockWidgetMovable);

  m_cfg = new ApplSettings();

  m_tabWidget = new QTabWidget(this);
  m_tabWidget->setObjectName("CentralWidget");

  setWidget(m_tabWidget);

  connect(this, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this,
          SLOT(dockPositionChanged(Qt::DockWidgetArea)));

  connect(m_tabWidget->tabBar(), SIGNAL(tabBarDoubleClicked(int)), this,
          SLOT(tabHideShowAction(int)));
}

/**
 * @brief DockBarWidget::dockPositionChanged
 * @short Wir verwenden das Andockfenster nur links und rechts.
 *        Tab Position je nach Seite ändern!
 * @param area
 */
void DockBarWidget::dockPositionChanged(Qt::DockWidgetArea area) {
  if (area == Qt::LeftDockWidgetArea) {
    m_tabWidget->setTabPosition(QTabWidget::West);
    m_cfg->setValue("dockarea/position", "left");
  } else if (area == Qt::RightDockWidgetArea) {
    m_tabWidget->setTabPosition(QTabWidget::East);
    m_cfg->setValue("dockarea/position", "right");
  }
}

/**
 * @brief DockBarWidget::tabHideShowAction
 * @todo Swap IN/OUT Function
 */
void DockBarWidget::tabHideShowAction(int index) {
  if (m_tabWidget->widget(index)->isVisible()) {
    qDebug() << Q_FUNC_INFO << "hide";
    // m_tabWidget->widget(index)->hide();
  } else {
    qDebug() << Q_FUNC_INFO << "show";
    // m_tabWidget->widget(index)->show();
  }
}
