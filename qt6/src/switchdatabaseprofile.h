// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUA_UI_SWITCHDATABASEPROFILE_H
#define ANTIQUA_UI_SWITCHDATABASEPROFILE_H

#include <AntiquaCRM>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QObject>
#include <QWidget>

class SwitchDatabaseProfile final : public QDialog {
  Q_OBJECT

private:
  AntiquaCRM::ASettings *config;
  QDialogButtonBox *btn_box;
  QComboBox *sql_profiles;

private Q_SLOTS:
  void saveAndQuit();

public:
  explicit SwitchDatabaseProfile(AntiquaCRM::ASettings *settings,
                                 QWidget *parent = nullptr);
  int exec() override;
};

#endif // ANTIQUA_UI_SWITCHDATABASEPROFILE_H
