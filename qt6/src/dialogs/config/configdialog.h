// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_DIALOGS_CONFIGDIALOG_H
#define ANTIQUACRM_DIALOGS_CONFIGDIALOG_H

#include <AntiquaTabs>
#include <AntiquaWidgets>
#include <QDialog>
#include <QDialogButtonBox>
#include <QEvent>
#include <QStatusBar>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

class ConfigGeneral;
class ConfigPaths;

class ConfigDialog final : public QDialog {
  Q_OBJECT

private:
  AntiquaCRM::ASettings *config;
  QVBoxLayout *layout;
  QTabWidget *m_pageView;
  QStatusBar *m_statusbar;
  QDialogButtonBox *m_buttonBox;

  ConfigGeneral *m_cfgGeneral;
  ConfigPaths *m_cfgPaths;

  const QList<AntiquaCRM::TabsConfigWidget *> groups();

  void closeEvent(QCloseEvent *) override;

  void loadConfigs();

private Q_SLOTS:
  void statusMessage(const QString &);

public Q_SLOTS:
  void aboutToSave();
  void aboutToClose();

public:
  explicit ConfigDialog(QWidget *parent = nullptr);
  ~ConfigDialog();

  int exec() override;
};

#endif
