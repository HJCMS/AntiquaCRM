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
#include <QFrame>
#include <QGroupBox>

/**
 * @class PaymentSettingsGroup
 * @brief Reserved for Currency, Tax and Price limits.
 */
class PaymentSettingsGroup final : public QFrame {
  Q_OBJECT

private:
  QGroupBox *m_taxGroup;
  QGroupBox *m_limitsGroup;

private Q_SLOTS:
  void updateCurrency();

public:
  /**
   * @brief Tax and Currency
   * @{
   */
  AntiquaCRM::SelectCurrency *m_currency;
  AntiquaCRM::NumEdit *m_vatNormal;
  AntiquaCRM::NumEdit *m_vatReduced;

  /**
   * @}
   *
   * @brief Price limits
   * @{
   */
  AntiquaCRM::PriceEdit *m_minPrice;
  AntiquaCRM::PriceEdit *m_normalPrice;

  /**
   * @}
   *
   * @param parent
   */
  explicit PaymentSettingsGroup(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_PAYMENT_SETTINGS_GROUP_H
