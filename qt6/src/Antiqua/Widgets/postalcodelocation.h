// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_POSTALCODELOCATION_H
#define ANTIQUACRM_WIDGETS_POSTALCODELOCATION_H

#include <AntiquaInput>
#include <QCompleter>
#include <QObject>
#include <QSqlField>
#include <QWidget>

namespace AntiquaCRM {

class PostalCodeEdit;

/**
 * @class PostalCodeLocation
 * @brief Display Postalcode locations
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY PostalCodeLocation final
    : public AntiquaCRM::AInputWidget {
  Q_OBJECT

private:
  /**
   * @brief location
   */
  ALineEdit *m_edit;

  /**
   * @brief location Completer
   */
  QCompleter *m_completer;

  /**
   * @brief History variable
   */
  QString p_history = QString();

  void initData() override;

private Q_SLOTS:
  /**
   * @brief Check history and compare with current entry
   *
   * This slot is triggerd by editingFinished and check changes from History.
   * if current data not equal set Window Modified and sendInputChanged emitted!
   *
   * The @ref p_history Variable is set in @ref setValue and cleared in @ref
   * reset function.
   */
  void updateChanged();

public Q_SLOTS:
  /**
   * @brief set location string
   */
  void setValue(const QVariant &value) override;

  /**
   * @brief Focused the cursor to the Line editor
   */
  void setFocus() override;

  /**
   * @brief Clears all inputs and reset internal changes to initialisation.
   */
  void reset() override;

public:
  /**
   * @param parent - parent Object
   */
  explicit PostalCodeLocation(QWidget *parent = nullptr);

  /**
   * @brief Create Completer for edit locations with PostalCode
   * This Slot is reserved to set Location and creates QCompleter from signal
   * „PostalCodeEdit::sendOnLeavePostalEdit“ data.
   * @param txobj - required for PostalCodeEdit::getLocations()
   * @param code - current PostalCode for query
   */
  bool setCompletion(AntiquaCRM::PostalCodeEdit *txobj,
                     const AntiquaCRM::PostalCode &code);

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

#endif // ANTIQUACRM_WIDGETS_POSTALCODELOCATION_H
