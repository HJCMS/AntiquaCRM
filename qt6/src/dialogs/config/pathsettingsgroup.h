// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PATH_SETTINGS_GROUP_H
#define ANTIQUACRM_PATH_SETTINGS_GROUP_H

#include <AntiquaWidgets>
#include <QFrame>
#include <QGroupBox>

class PathSettingsGroup : public QFrame {
  Q_OBJECT

private:
  inline const QString configPath(const QString &) const;

public:
  AntiquaCRM::SelectTargets *m_setDelivery;
  AntiquaCRM::SelectTargets *m_setInvoices;
  AntiquaCRM::SelectTargets *m_setReminder;
  AntiquaCRM::SelectTargets *m_setReports;
  AntiquaCRM::SelectTargets *m_setCards;
  AntiquaCRM::SelectTargets *m_setImageArchive;
  AntiquaCRM::SelectTargets *m_setImageImport;

  explicit PathSettingsGroup(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_PATH_SETTINGS_GROUP_H
