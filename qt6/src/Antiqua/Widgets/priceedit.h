// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_PRICEEDIT_H
#define ANTIQUACRM_WIDGETS_PRICEEDIT_H

#include <AntiquaInput>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class PriceEdit
 * @brief Price Input edit widget
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY PriceEdit final : public AntiquaCRM::AbstractInput {
  Q_OBJECT

private:
  AntiquaCRM::ADoubleBox *m_edit;

  /**
   * @brief Try - to convert a given money string into double value.
   */
  bool fromMoneyString(const QString &) const;

private Q_SLOTS:
  void valueChanged(double);

protected:
  void initData() override;

public Q_SLOTS:
  void setValue(const QVariant &) override;

  void setFocus() override;

  void reset() override;

public:
  /**
   * @param min    - minimum price
   * @param max    - maximum price
   * @param parent - parent widget
   */
  explicit PriceEdit(double min, double max, QWidget *parent = nullptr);

  /**
   * @brief Price Input edit widget
   * @param parent - parent widget
   */
  explicit PriceEdit(QWidget *parent = nullptr);

  /**
   * @brief Configure data restriction from QSqlQuery fieldset.
   */
  void setRestrictions(const QSqlField &) override;

  /**
   * @brief Configure minimum price
   */
  void setMinimum(double min);

  /**
   * @brief Configure maximum price
   */
  void setMaximum(double max);

  /**
   * @brief Configure range from minimum to maximum price
   */
  void setRange(double min, double max);

  /**
   * @brief Configure single steps
   */
  void setSingleStep(double steps);

  void setInputToolTip(const QString &) override;

  void setBuddyLabel(const QString &) override;

  bool isValid() override;

  /**
   * @brief what is the currency symbol for this input
   * used by AntiquaCRM::ADoubleBox::setSuffix
   */
  const QString currencySymbol() const;

  const QMetaType getType() const override;

  const QVariant getValue() override;

  /**
   * @brief returning a money string with currency symbol
   */
  const QString getMoney() const;

  const QString popUpHints() override;

  const QString statusHints() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_PRICEEDIT_H
