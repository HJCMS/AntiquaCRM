// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PROVIDER_ACTION_DIALOG_H
#define ANTIQUACRM_PROVIDER_ACTION_DIALOG_H

#include <AGlobal>
#include <QAbstractButton>
#include <QCloseEvent>
#include <QDialog>
#include <QDialogButtonBox>
#include <QJsonObject>
#include <QStackedWidget>
#include <QStatusBar>
#include <QUrl>
#include <QWidget>

namespace AntiquaCRM {

class ProviderActionNavigator;

/**
 * @class ProviderActionDialog
 * @brief Abstract Dialog for Provider Remote actions.
 *
 * @ingroup ProviderInterface
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
   * @brief isWindowModified, prevent close requests with system window bars
   */
  void closeEvent(QCloseEvent *) override final;

  /**
   * @brief fetch status tips from links and buttons
   * @return
   */
  virtual bool event(QEvent *) override;

  /**
   * @brief Default Status Bar Reject message
   */
  void setRejectMessage();

private Q_SLOTS:
  void buttonAction(QAbstractButton *button);

protected:
  /**
   * @brief Central design widget
   */
  QStackedWidget *stackedWidget;

  /**
   * @brief Pages Navigator
   */
  ProviderActionNavigator *m_navigator;

  /**
   * @brief Read Configuration
   */
  virtual bool initConfiguration() = 0;

  /**
   * @brief API query string
   */
  virtual const QUrl apiQuery(const QString &) = 0;

  /**
   * @brief protected for invalid public access
   */
  int exec() final override;

protected Q_SLOTS:
  /**
   * @brief Redirection slot to setWindowModified
   */
  void isChildWindowModified(bool);

Q_SIGNALS:
  /**
   * @brief emitted when apply button clicked
   */
  void sendSubmitClicked();

public Q_SLOTS:
  /**
   * @brief Forward messages to statusbar
   * @param message - Message string
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
   * @param remote - enable submit remote actions button
   */
  explicit ProviderActionDialog(QWidget *parent = nullptr, bool remote = true);

  /**
   * @brief Open Dialog with current Provider order data.
   * @param data - dataset for operations
   */
  virtual int exec(const QJsonObject &data) = 0;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_PROVIDER_ACTION_DIALOG_H
