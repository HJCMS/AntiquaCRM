// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_SELECTORDERSTATUS_H
#define ANTIQUACRM_WIDGETS_SELECTORDERSTATUS_H

#include <AntiquaInput>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class SelectOrderStatus
 * @brief Select Method for Orders status.
 *
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY SelectOrderStatus final
    : public AntiquaCRM::AInputWidget {
  Q_OBJECT

private:
  AComboBox *m_edit;
  AntiquaCRM::OrderStatus p_onload_status;

private Q_SLOTS:
  void valueChanged(int);

protected:
  void initData() override;

Q_SIGNALS:
  void sendStatusChanged(AntiquaCRM::OrderStatus);

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
  explicit SelectOrderStatus(QWidget *parent = nullptr);

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

#endif // ANTIQUACRM_WIDGETS_SELECTORDERSTATUS_H
