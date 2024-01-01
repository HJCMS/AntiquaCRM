// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_DIALOGS_DATABASE_PROFILE_H
#define ANTIQUACRM_DIALOGS_DATABASE_PROFILE_H

#include <AntiquaWidgets>
#include <QFrame>
#include <QPair>
#include <QPushButton>

class DatabaseProfile final : public QFrame {
  Q_OBJECT

private:
  AntiquaCRM::ASettings *cfg;
  const QString p_profil;
  AntiquaCRM::AComboBox *m_select;
  QPushButton *m_create;
  QPushButton *m_remove;
  QPushButton *m_test;
  static const QIcon entryIcon();

private Q_SLOTS:
  void actionCreate();
  void actionRemove();
  void setProfile(int index);

Q_SIGNALS:
  void sendUpdateProfile();
  void sendStartTest();

public Q_SLOTS:
  void findProfile(const QString &name);
  void loadEntries();
  void setTestFocus();

public:
  explicit DatabaseProfile(AntiquaCRM::ASettings *config,
                           QWidget *parent = nullptr);
  const QString currentProfile();
  const QString currentIdentifier();
};

#endif // ANTIQUACRM_DIALOGS_DATABASE_PROFILE_H
