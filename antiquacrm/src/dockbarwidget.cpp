#include "dockbarwidget.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtCore/QPropertyAnimation>
#include <QtCore/QSize>
#include <QtWidgets/QTabBar>

DockBarWidget::DockBarWidget(QWidget *parent) : QDockWidget{parent} {
  setObjectName("DockAreaWidget");
  setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  setFeatures(QDockWidget::DockWidgetMovable);

  m_cfg = new ApplSettings();

  m_tabWidget = new QTabWidget(this);
  m_tabWidget->setObjectName("CentralWidget");

  m_search = new QWidget(m_tabWidget);
  m_search->setObjectName("pro_search");
  m_tabWidget->addTab(m_search, myIcon("tab"), tr("Search"));
/*
  QRect s = m_search->rect();
  QPropertyAnimation animation(m_search, "geometry");
  animation.setDuration(20000);
  animation.setStartValue(QRect(s.topRight(), QSize(2, s.height())));
  animation.setEndValue(QRect(s.topLeft(), QSize(262, s.height())));
  animation.start();
  qDebug() << s << s.topRight() << m_search->rect();
*/
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
void DockBarWidget::tabHideShowAction(int index) {}
