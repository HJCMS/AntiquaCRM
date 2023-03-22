// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_GENDEREDIT_H
#define ANTIQUACRM_WIDGETS_GENDEREDIT_H

#include <AntiquaInput>
#include <QHash>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @ingroup AntiquaWidgets
 * @class GenderEdit
 */
class ANTIQUACRM_LIBRARY GenderEdit final : public AntiquaCRM::AbstractInput {
  Q_OBJECT

private:
  AntiquaCRM::AComboBox *m_edit;

private Q_SLOTS:
  /**
   * @brief price has been changed
   */
  void valueChanged(int);

protected:
  /**
   * @brief create basic dataset construction
   * @note Field name or Object name is not set in this function.
   */
  virtual void initData() override;

public Q_SLOTS:
  /**
   * @brief set gender
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
   * @brief Gender Selecter
   * @param parent - parent widget
   */
  explicit GenderEdit(QWidget *parent = nullptr);

  /**
   * @brief Current gender Dataset
   */
  static const QHash<AntiquaCRM::Gender, QString> genderData();

  /**
   * @brief Configure data restriction from QSqlQuery fieldset.
   */
  virtual void setRestrictions(const QSqlField &) override;

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
   * @brief get current price
   */
  virtual const QVariant getValue() override;

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

#endif // ANTIQUACRM_WIDGETS_GENDEREDIT_H
