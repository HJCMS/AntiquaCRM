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
  ALineEdit *m_edit;       /**< @brief location */
  QCompleter *m_completer; /**< @brief location Completer */
  void initData() override;

public Q_SLOTS:
  /**
   * @brief set location string
   */
  void setValue(const QVariant &value) override;

  void setFocus() override;

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
