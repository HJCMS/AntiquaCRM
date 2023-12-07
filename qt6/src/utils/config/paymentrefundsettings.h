// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PAYMENT_REFUND_SETTINGS_H
#define ANTIQUACRM_PAYMENT_REFUND_SETTINGS_H

#include <AntiquaWidgets>
#include <QFrame>
#include <QLabel>
#include <QGroupBox>
#include <QJsonObject>

class PaymentRefundSettings : public QFrame {
  Q_OBJECT

private:
  double p_min_cost = 0.00;
  double p_max_cost = 9999.00;
  QGroupBox *m_groupBox;
  AntiquaCRM::PriceEdit *m_default;
  AntiquaCRM::PriceEdit *m_books;
  AntiquaCRM::PriceEdit *m_prints;
  AntiquaCRM::PriceEdit *m_media;
  AntiquaCRM::PriceEdit *m_various;

  QLabel *infoWidget(const QString &text);
  AntiquaCRM::PriceEdit *editWidget(const QString &name);

public Q_SLOTS:
  void setConfig(const QJsonObject &);

public:
  explicit PaymentRefundSettings(QWidget *parent = nullptr);
  const QJsonObject getConfig();
};

#endif // ANTIQUACRM_PAYMENT_REFUND_SETTINGS_H
