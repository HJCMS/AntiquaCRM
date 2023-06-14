// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_POSTALCODESTATE_H
#define ANTIQUACRM_WIDGETS_POSTALCODESTATE_H

#include <AntiquaInput>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class PostalCodeState
 * @brief Display Postalcode country/state
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY PostalCodeState final
    : public AntiquaCRM::AInputWidget {
  Q_OBJECT

private:
  ALineEdit *m_edit;
  void initData() override;

public Q_SLOTS:
  /**
   * @brief Set Country and State from signal.
   * This Slot is reserved to set Country and State from Signal
   * AntiquaCRM::PostalCodeEdit::sendOnLeavePostalEdit
   */
  void setCountry(const AntiquaCRM::PostalCode &);

  /**
   * @brief Set value to Country/State.
   * @code
   *  Different output formats are used:
   *    Österreich
   *    Deutschland/Schleswig-Holstein
   * @endcode
   * The gradations will be needed later for statistic charts.
   */
  void setValue(const QVariant &) override;

  void setFocus() override;

  void reset() override;

public:
  /**
   * @param parent - parent Object
   */
  explicit PostalCodeState(QWidget *parent = nullptr);

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

#endif // ANTIQUACRM_WIDGETS_POSTALCODESTATE_H
