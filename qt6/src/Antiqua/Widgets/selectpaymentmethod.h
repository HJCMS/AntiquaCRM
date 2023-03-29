// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_SELECTPAYMENTMETHOD_H
#define ANTIQUACRM_WIDGETS_SELECTPAYMENTMETHOD_H

#include <AntiquaInput>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class SelectPaymentMethod
 * @brief Select Method for Provider payments.
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY SelectPaymentMethod final
    : public AntiquaCRM::AbstractInput {
  Q_OBJECT

private:
  QMap<int, QPair<QString, AntiquaCRM::PaymentMethod>> p_map;
  AComboBox *m_edit;

private Q_SLOTS:
  void valueChanged(int);

protected:
  void initData() override;

Q_SIGNALS:
  void sendPaymentMethodChanged(AntiquaCRM::PaymentMethod);

public Q_SLOTS:
  void setValue(const QVariant &) override;

  void setFocus() override;

  void reset() override;

  void setReadOnly(bool b = false);

public:
  /**
   * @param parent - parent widget
   */
  explicit SelectPaymentMethod(QWidget *parent = nullptr);

  void setRestrictions(const QSqlField &) override;

  void setInputToolTip(const QString &) override;

  void setBuddyLabel(const QString &) override;

  bool isValid() override;

  const QVariant getValue() override;

  const QString popUpHints() override;

  const QString statusHints() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_SELECTPAYMENTMETHOD_H
