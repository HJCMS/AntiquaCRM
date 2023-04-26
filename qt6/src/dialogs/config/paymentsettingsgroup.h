// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PAYMENT_SETTINGS_GROUP_H
#define ANTIQUACRM_PAYMENT_SETTINGS_GROUP_H

#include <AntiquaWidgets>
#include <QGroupBox>

class PaymentSettingsGroup final : public QGroupBox {
  Q_OBJECT

public:
  AntiquaCRM::SelectCurrency *m_currency;
  AntiquaCRM::NumEdit *m_vatNormal;
  AntiquaCRM::NumEdit *m_vatReduced;

  explicit PaymentSettingsGroup(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_PAYMENT_SETTINGS_GROUP_H
