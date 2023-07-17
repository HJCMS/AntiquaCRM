// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_DIALOGS_CONFIGDIALOG_H
#define ANTIQUACRM_DIALOGS_CONFIGDIALOG_H

#include <AntiquaWidgets>
#include <QDialog>
#include <QDialogButtonBox>
#include <QEvent>
#include <QStatusBar>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

class ConfigPagesWidget;
class ConfigTreeWidget;
class ConfigGeneral;
class ConfigDatabase;
class ConfigPaths;
class ConfigPrinting;
class ConfigLookAndFeel;
class ConfigTabsView;
class ConfigProvidersView;

class ConfigDialog final : public QDialog {
  Q_OBJECT

private:
  AntiquaCRM::ASettings *config;
  ConfigTreeWidget *m_treeWidget;
  ConfigPagesWidget *m_pageView;
  QStatusBar *m_statusbar;
  QDialogButtonBox *m_buttonBox;

  ConfigGeneral *m_cfgGeneral;
  ConfigDatabase *m_cfgDatabase;
  ConfigPaths *m_cfgPaths;
  ConfigPrinting *m_cfgPrinter;
  ConfigLookAndFeel *m_cfgLookAndFeel;

  ConfigTabsView *m_cfgTabs;
  ConfigProvidersView *m_cfgProviders;

  void closeEvent(QCloseEvent *) override;

  bool loadConfigWidget();

  void loadConfigs();

private Q_SLOTS:
  void setModified(bool);
  void updateTitle(const QString &);
  void statusMessage(const QString &message);
  void openConfigGroup(const QString &name);

public Q_SLOTS:
  void aboutToSave();
  void aboutToClose();

public:
  explicit ConfigDialog(QWidget *parent = nullptr);
  ~ConfigDialog();

  int exec() override;
};

#endif
