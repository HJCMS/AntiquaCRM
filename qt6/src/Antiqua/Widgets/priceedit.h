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
 * @ingroup AntiquaWidgets
 * @class PriceEdit
 */
class ANTIQUACRM_LIBRARY PriceEdit final : public AntiquaCRM::AbstractInput {
  Q_OBJECT

private:
  AntiquaCRM::ADoubleBox *m_edit;

  /**
   * @brief Try - to convert a given money string into double value.
   */
  bool setFromString(const QString &) const;

private Q_SLOTS:
  /**
   * @brief price has been changed
   */
  void valueChanged(double);

protected:
  /**
   * @brief create basic dataset construction
   * @note Field name or Object name is not set in this function.
   */
  virtual void initData(const QVariant &param = QVariant()) override;

public Q_SLOTS:
  /**
   * @brief set price
   */
  virtual void setValue(const QVariant &) override;

  /**
   * @brief set input focus
   */
  virtual void setFocus() override;

  /**
   * @brief restore to minium value
   */
  virtual void reset() override;

public:
  /**
   * @brief Price Input edit widget
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
  virtual void setRestrictions(const QSqlField &) override;

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

  /**
   * @brief Configure tooltip for this input edit widget
   */
  virtual void setInputToolTip(const QString &) override;

  /**
   * @brief Add a buddy label to this input widget.
   */
  virtual void setBuddyLabel(const QString &) override;

  /**
   * @brief is it a valid price input
   */
  virtual bool isValid() override;

  /**
   * @brief what is the currency symbol for this input
   * used by AntiquaCRM::ADoubleBox::setSuffix
   */
  const QString currencySymbol() const;

  /**
   * @brief get current price
   */
  virtual const QVariant getValue() override;

  /**
   * @brief returning a money string with currency symbol
   */
  const QString getMoney() const;

  /**
   * @brief Detailed warn message for QMessageBox
   */
  virtual const QString popUpHints() override;

  /**
   * @brief Simple warn message for QStatusBar
   */
  virtual const QString statusHints() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_PRICEEDIT_H
