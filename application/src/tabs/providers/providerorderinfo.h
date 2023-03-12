// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_PROVIDERORDERINFO_H
#define ANTIQUACRM_PROVIDERORDERINFO_H

#include <AntiquaWidgets>
#include <QDateTime>
#include <QObject>
#include <QLabel>
#include <QWidget>
#include <QJsonObject>

/**
 * @brief The ProviderOrderInfo class
 * @ingroup _providers
 */
class ProviderOrderInfo : public QWidget {
  Q_OBJECT

private:
  AntiquaDateInfo *m_since;
  QLabel *m_paymentMethod;
  QLabel *m_orderPayed;
  AntiquaDateInfo *m_paymentConfirmed;
  QLabel *infoCell(const QString &);
  const QDateTime convertDateTime(const QString &);
  void setPaymentMethod(int);

public:
  explicit ProviderOrderInfo(QWidget *parent = nullptr);
  void setOrderInfo(const QJsonObject &info);
};

#endif // ANTIQUACRM_PROVIDERORDERINFO_H
