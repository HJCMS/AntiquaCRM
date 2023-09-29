// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_CONFIGMENUS_H
#define ANTIQUACRM_CONFIGMENUS_H

#include <QAction>
#include <QMenu>
#include <QObject>
#include <QMenuBar>

/**
 * @ingroup ui
 * @brief AntiquaCRM configuration menus
 */
class AntiquaConfigMenus final : public QMenu {
  Q_OBJECT

private:
  QMenu *m_tables;
  QAction *ac_company;
  QAction *ac_keywords;
  QAction *ac_designations;
  QAction *ac_publishers;
  QAction *ac_storage;
  QAction *ac_delivery;
  QAction *ac_templates;
  QAction *ac_system;

private Q_SLOTS:
  void openConfigDialog();
  void openCompanyDialog();
  void openKeywordsDialog();
  void openDesignationsDialog();
  void openStorageDialog();
  void openDeliveryDialog();
  void openTemplatesDialog();
  void openBookPublisherDialog();

public:
  explicit AntiquaConfigMenus(QMenuBar *parent);
};

#endif // ANTIQUACRM_CONFIGMENUS_H
