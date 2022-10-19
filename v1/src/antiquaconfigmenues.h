// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_CONFIGMENUES_H
#define ANTIQUACRM_CONFIGMENUES_H

#include <QAction>
#include <QMenu>
#include <QObject>
#include <QWidget>

class AntiquaConfigMenues final : public QMenu {
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
  explicit AntiquaConfigMenues(QWidget *parent);
};

#endif // ANTIQUACRM_CONFIGMENUES_H
