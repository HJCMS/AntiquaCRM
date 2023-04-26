// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_SELECTPAYMENTSTATUS_H
#define ANTIQUACRM_WIDGETS_SELECTPAYMENTSTATUS_H

#include <AntiquaInput>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @ingroup AntiquaWidgets
 * @class SelectPaymentStatus
 */
class ANTIQUACRM_LIBRARY SelectPaymentStatus final
    : public AntiquaCRM::AbstractInput {
  Q_OBJECT

private:
  AComboBox *m_edit;
  void initData() override;

private Q_SLOTS:
  void valueChanged(int);

Q_SIGNALS:
  void sendPaymentStatusChanged(AntiquaCRM::ProviderPaymentStatus);

public Q_SLOTS:
  void setValue(const QVariant &) override;

  void setFocus() override;

  void reset() override;

public:
  /**
   * @brief __TODO__
   * @param parent - parent widget
   */
  explicit SelectPaymentStatus(QWidget *parent = nullptr);

  void setRestrictions(const QSqlField &) override;

  void setInputToolTip(const QString &) override;

  void setBuddyLabel(const QString &) override;

  bool isValid() override;

  const QMetaType getType() const override;

  const QVariant getValue() override;

  const QString popUpHints() override;

  const QString statusHints() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_SELECTPAYMENTSTATUS_H
