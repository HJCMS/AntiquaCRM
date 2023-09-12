// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_PROVIDERSPAGEVIEW_H
#define ANTIQUACRM_PROVIDERSPAGEVIEW_H

#include <AntiquaCRM>
#include <AntiquaWidgets>
#include <QObject>
#include <QPaintEvent>
#include <QTabWidget>
#include <QWidget>

class ProvidersOrderPage;

/**
 * @brief Inventory Providers Tab widget
 * @ingroup _providers
 */
class ProvidersPageView final : public QTabWidget {
  Q_OBJECT

private:
  AntiquaTabBar *m_tabBar;
  inline const QString infoText() const;
  virtual void paintEvent(QPaintEvent *) override;

private Q_SLOTS:
  void closeTabClicked(int);
  void pageEntered(int);

Q_SIGNALS:
  void sendCloseTab(int index);
  void sendPageChanged();

public:
  explicit ProvidersPageView(QWidget *parent);

  /**
   * @brief Aktuelles Tab Fenster ermitteln.
   * Ableitung von QTabWidget::currentWidget()
   */
  ProvidersOrderPage *currentPage();

  /**
   * @brief Bestellerseite
   */
  int addPage(ProvidersOrderPage *aiw, const QString &id);

  void addFixedTab(QWidget *tab, const QString &title);

  /**
   * @brief Beschreibung für Tooltip ändern!
   */
  void setTabDescription(int index, const QString &);
};

#endif // ANTIQUACRM_PROVIDERSPAGEVIEW_H
