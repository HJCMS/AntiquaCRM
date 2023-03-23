// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_SALUTATIONEDIT_H
#define ANTIQUACRM_WIDGETS_SALUTATIONEDIT_H

#include <AntiquaInput>
#include <QMap>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @ingroup AntiquaWidgets
 * @class SalutationEdit
 */
class ANTIQUACRM_LIBRARY SalutationEdit final
    : public AntiquaCRM::AbstractInput {
  Q_OBJECT

private:
  AntiquaCRM::AComboBox *m_edit;

private Q_SLOTS:
  /**
   * @brief salutation has been changed
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
   * @brief set salutation
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
   * @brief Salutation selecter class
   * @param parent - parent widget
   */
  explicit SalutationEdit(QWidget *parent = nullptr);

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
   * @brief is a valid salutation set
   */
  virtual bool isValid() override;

  /**
   * @brief get current value
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

  /**
   * @brief get all salutations from json document
   */
  static const QMap<QString, QString> salutations();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_SALUTATIONEDIT_H
