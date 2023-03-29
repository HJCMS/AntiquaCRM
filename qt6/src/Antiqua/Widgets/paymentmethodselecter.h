// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_PAYMENTMETHODSELECTER_H
#define ANTIQUACRM_WIDGETS_PAYMENTMETHODSELECTER_H

#include <AntiquaInput>
#include <QMap>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class PaymentMethodSelecter
 * @brief Select Method for Provider payments.
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY PaymentMethodSelecter final
    : public AntiquaCRM::AbstractInput {
  Q_OBJECT

private:
  QMap<int, QPair<QString, AntiquaCRM::PaymentMethod>> p_map;
  AComboBox *m_edit;

private Q_SLOTS:
  void valueChanged(int);

protected:
  void initData() override;

public Q_SLOTS:
  void setValue(const QVariant &) override;

  void setFocus() override;

  void reset() override;

Q_SIGNALS:
  void sendPaymentMethodChanged(AntiquaCRM::PaymentMethod);

public:
  /**
   * @param parent - parent widget
   */
  explicit PaymentMethodSelecter(QWidget *parent = nullptr);

  void setRestrictions(const QSqlField &) override;

  void setInputToolTip(const QString &) override;

  void setBuddyLabel(const QString &) override;

  bool isValid() override;

  const QVariant getValue() override;

  const QString popUpHints() override;

  const QString statusHints() override;

  static const QMap<AntiquaCRM::PaymentMethod,QString> getMethodes();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_PAYMENTMETHODSELECTER_H
