// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_CONFIGMENUS_H
#define ANTIQUACRM_CONFIGMENUS_H

#include <QAction>
#include <QMenu>
#include <QObject>
#include <QWidget>

class AntiquaConfigMenus final : public QMenu {
  Q_OBJECT

private:
  QMenu *m_tables;
  QAction *ac_keywords;
  QAction *ac_storage;
  QAction *ac_delivery;
  QAction *ac_system;

private Q_SLOTS:
  void openConfigDialog();
  void openKeywordsDialog();
  void openStorageDialog();
  void openDeliveryDialog();

public:
  explicit AntiquaConfigMenus(QWidget *parent);
};

#endif // ANTIQUACRM_CONFIGMENUS_H
