// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_POPUP_MESSAGE_H
#define ANTIQUACRM_POPUP_MESSAGE_H

#include <AGlobal>
#include <QMessageBox>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class APopUpMessage
 * @brief Antiqua CRM Message box
 *
 * This class use QDialog::DialogCode with QMessageBox::StandardButtons.
 * In this case, we use custom buttons.
 *
 * @note Please read also api description from QMessageBox::exec()
 *
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY APopUpMessage final : public QMessageBox {
  Q_OBJECT

public:
  /**
   * @param parent - a popup must have a parent object
   */
  explicit APopUpMessage(QWidget *parent);

  /**
   * @brief Create Error message in plaintText
   * @param error   - general message
   * @param details - hidden details
   */
  void setErrorMessage(const QString &error, const QString &details);

  /**
   * @brief Open Warning PopUp
   * @param title - PopUp Title
   * @param body  - Body
   */
  void setWarnMessage(const QString &title, const QString &body);

  /**
   * @brief Open warning PopUp
   * @param body  - Body
   */
  void setNoticeMessage(const QString &body);

  /**
   * @brief Open save success PopUp
   * @param body  - Body
   */
  void setSuccessMessage(const QString &body);

  /**
   * @brief Open Unsaved changes PopUp
   * @param body - PopUp Message
   * @param ask  - Show Yes|No Buttons, otherwise only Ok is displayed!
   */
  void setUnsavedMessage(const QString &body, bool ask = false);

  /**
   * @brief This will always return QDialog::DialogCode
   *
   * Only (Ok|Yes|Ignore) will send QDialog::Accepted, any other
   * QMessageBox::StandardButtons will return QDialog::Rejected!
   */
  int exec() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_POPUP_MESSAGE_H
