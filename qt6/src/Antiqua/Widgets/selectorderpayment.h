// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_SELECTORDERPAYMENT_H
#define ANTIQUACRM_WIDGETS_SELECTORDERPAYMENT_H

#include <AntiquaInput>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class SelectOrderPayment
 * @brief Select Method for Order Payment status.
 *
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY SelectOrderPayment final
    : public AntiquaCRM::AInputWidget {
  Q_OBJECT

private:
  AComboBox *m_edit;
  AntiquaCRM::OrderPayment p_onload_status;

private Q_SLOTS:
  void valueChanged(int);

protected:
  void initData() override;

Q_SIGNALS:
  void sendStatusChanged(AntiquaCRM::OrderPayment);

public Q_SLOTS:
  void setValue(const QVariant &) override;

  void setFocus() override;

  void reset() override;

  void setReadOnly(bool b = false);

  /**
   * @brief Reject last operation and setValue(p_onload_status)
   */
  void setReject();

public:
  /**
   * @param parent - parent widget
   */
  explicit SelectOrderPayment(QWidget *parent = nullptr);

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

#endif // ANTIQUACRM_WIDGETS_SELECTORDERPAYMENT_H
