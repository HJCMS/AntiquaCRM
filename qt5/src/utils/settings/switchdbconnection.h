// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUA_CONFIG_SWITCH_DATABASE_H
#define ANTIQUA_CONFIG_SWITCH_DATABASE_H

#include <AntiquaCRM>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QObject>
#include <QWidget>

class SwitchDBConnection final : public QDialog {
  Q_OBJECT

private:
  AntiquaCRM::ASettings *cfg;
  QDialogButtonBox *btn_box;
  QComboBox *sql_profiles;

private Q_SLOTS:
  void saveAndQuit();

public:
  explicit SwitchDBConnection(QWidget *parent = nullptr);
  int exec() override;
};

#endif // ANTIQUA_CONFIG_SWITCH_DATABASE_H
