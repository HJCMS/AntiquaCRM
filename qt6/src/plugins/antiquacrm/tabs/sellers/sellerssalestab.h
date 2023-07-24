// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_SELLERS_SALESTAB_H
#define ANTIQUACRM_PLUGIN_SELLERS_SALESTAB_H

#include <AntiquaTabs>
#include <AntiquaWidgets>
#include <QTabWidget>

class SellersSalesWidget;

class ANTIQUACRM_LIBRARY SellersSalesTab final : public QTabWidget {
  Q_OBJECT

private:
  AntiquaCRM::TabsBar *m_tabsBar;

public:
  explicit SellersSalesTab(QWidget *parent = nullptr);

  /**
   * @brief Aktuelles Tab Fenster ermitteln.
   * Ableitung von QTabWidget::currentWidget()
   */
  SellersSalesWidget *currentPage();

  int addPage(const QString &id, SellersSalesWidget *page);
};

#endif
