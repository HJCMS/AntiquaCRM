// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_PROVIDERS_PAGEVIEW_H
#define INVENTORY_PROVIDERS_PAGEVIEW_H

#include <AntiquaInterface>
#include <QContextMenuEvent>
#include <QTabBar>
#include <QTabWidget>

class ProvidersTabBar : public QTabBar {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  int index = -1;

private Q_SLOTS:
  void checkToClose();

protected:
  void tabInserted(int index);
  void contextMenuEvent(QContextMenuEvent *event);

Q_SIGNALS:
  void s_closeTab(int index);

public:
  explicit ProvidersTabBar(QTabBar *parent);
};

class ProvidersPageView : public QTabWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  ProvidersTabBar *m_tabBar;

private Q_SLOTS:
  void closeTabClicked(int);
  void pageEntered(int);

Q_SIGNALS:
  void orderPageChanged();

public:
  explicit ProvidersPageView(QWidget *parent);

  /**
   * @brief Aktuelles Tab Fenster ermitteln.
   * Ableitung von QTabWidget::currentWidget()
   * Ermittelt das aktuelles Antiqua::InterfaceWidget
   */
  Antiqua::InterfaceWidget *currentPage();

  /**
   * @brief Bestellerseite
   */
  int addPage(Antiqua::InterfaceWidget *aiw, const QString &id);
};

#endif // INVENTORY_PROVIDERS_PAGEVIEW_H
