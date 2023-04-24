// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_CURRENCYSELECTOR_H
#define ANTIQUACRM_WIDGETS_CURRENCYSELECTOR_H

#include <AntiquaInput>
#include <QLocale>
#include <QMap>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class CurrencySelector
 * @brief The „CurrencySelector“ input edit class.
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY CurrencySelector final
    : public AntiquaCRM::AbstractInput,
      private QLocale {
  Q_OBJECT

private:
  AComboBox *m_box;
  ALineEdit *m_edit;

private Q_SLOTS:
  /**
   * @brief Internal changes when data has changed
   */
  void valueChanged(int);

protected:
  /**
   * @brief load Database or System file data for on load
   */
  void initData() override;

public Q_SLOTS:
  /**
   * @brief set Value slot
   */
  void setValue(const QVariant &) override;

  /**
   * @brief Set focus to Input Editor
   */
  void setFocus() override;

  /**
   * @brief Reset all Input fields and restore Window Modified
   */
  void reset() override;

public:
  /**
   * @param parent - parent widget
   */
  explicit CurrencySelector(QWidget *parent = nullptr);

  /**
   * @brief Restrict input wirt QSqlField parameters.
   * With this function the Input Field can restricted.
   * The QSqlField class contains all Parameters to configure a Input field.
   */
  void setRestrictions(const QSqlField &) override;

  /**
   * @brief Append Tooltip Icon with Description text.
   * If Empty nothing will done.
   */
  void setInputToolTip(const QString &) override;

  /**
   * @brief Prepend a Textlabel with Buddy Focus.
   */
  void setBuddyLabel(const QString &) override;

  /**
   * @brief Validate Input with given Parameters from setRestrictions.
   */
  bool isValid() override;

  /**
   * @brief get current Data ...
   */
  const QVariant getValue() override;

  /**
   * @brief Reserved for QMessageBox poup Windows.
   */
  const QString popUpHints() override;

  /**
   * @brief Reserved for QStatusBar messages.
   */
  const QString statusHints() override;

  /**
   * @brief Primary Cuerrency Territories
   */
  static const QMap<QLocale::Territory, QString> cuerrencies();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_CURRENCYSELECTOR_H
