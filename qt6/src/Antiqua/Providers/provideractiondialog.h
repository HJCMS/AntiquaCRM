// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PROVIDER_ACTION_DIALOG_H
#define ANTIQUACRM_PROVIDER_ACTION_DIALOG_H

#include <AGlobal>
#include <QAbstractButton>
#include <QBoxLayout>
#include <QCloseEvent>
#include <QDialog>
#include <QDialogButtonBox>
#include <QJsonObject>
#include <QScrollArea>
#include <QStatusBar>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class ProviderActionDialog
 * @brief Abstract Dialog for Provider Remote actions.
 *
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY ProviderActionDialog : public QDialog {
  Q_OBJECT

private:
  /**
   * @brief ButtonBox
   */
  QDialogButtonBox *m_buttonBox;

  /**
   * @brief StatusBar
   */
  QStatusBar *m_statusBar;

  /**
   * @brief prevent close with changes
   */
  void closeEvent(QCloseEvent *) override;

private Q_SLOTS:
  void buttonAction(QAbstractButton *button);

protected:
  /**
   * @brief Central design widget
   */
  QScrollArea *m_scrollArea;

  /**
   * @brief prevent invalid access
   */
  int exec() final override;

Q_SIGNALS:
  /**
   * @brief emitted when apply button clicked
   */
  void sendApplyClicked();

  /**
   * @brief emitted when ok button clicked
   */
  void sendOkClicked();

  /**
   * @brief emitted when cancel button clicked
   */
  void sendCancelClicked();

  /**
   * @brief emitted when close button clicked
   */
  void sendCloseClicked();

public Q_SLOTS:
  /**
   * @brief Forward messages to statusbar
   * @param message - Body
   */
  void statusMessage(const QString &message);

  /**
   * @brief prepare plugin operations
   *
   * typically used by onclick submit buttons
   */
  virtual void prepareOperation() = 0;

public:
  /**
   * @param parent - parent object
   */
  explicit ProviderActionDialog(QWidget *parent = nullptr);

  /**
   * @brief Open with current Provider order data.
   * @param data - dataset for operations
   */
  virtual int exec(const QJsonObject &data) = 0;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_PROVIDER_ACTION_DIALOG_H
