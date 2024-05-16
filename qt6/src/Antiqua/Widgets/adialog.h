// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_DIALOG_H
#define ANTIQUACRM_WIDGETS_DIALOG_H

#include <AGlobal>
#include <QCloseEvent>
#include <QDialog>
#include <QDialogButtonBox>
#include <QEvent>
#include <QKeyEvent>
#include <QLayout>
#include <QObject>
#include <QPushButton>
#include <QStatusBar>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class ADialog
 * @brief Default AntiquaCRM Dialog ...
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY ADialog : public QDialog {
  Q_OBJECT

protected:
  /**
   * @brief Register signals, editor reports unsaved records!
   *
   * This event filter search for isWindowModified in AntiquaCRM::ADialog
   * classes and have installed manual in subclass from this.
   *
   * @code
   * m_widget = new QWidget(this);
   * m_widget->installEventFilter(this);
   * @endcode
   */
  virtual bool eventFilter(QObject *, QEvent *) override final;

  /**
   * @brief Layout for sub classing
   *
   * Default vertical layout for sub classes.
   *
   * @note The Main Layout is a privat QBoxLayout
   */
  QBoxLayout *layout;

  /**
   * @brief Default ButtonsBar
   */
  QDialogButtonBox *m_buttonsBar;

  /**
   * @brief Close button without signal processing.
   *
   * It can modify in subclasses ...
   */
  QPushButton *btn_apply, *btn_reject;

  /**
   * @brief Default StatusBar
   */
  QStatusBar *m_statusBar;

  /**
   * @brief Catching key press enter or key return events.
   *
   * Preventing accept/close request by keyboard modifiers.
   */
  virtual void keyPressEvent(QKeyEvent *) override;

  /**
   * @brief Sending all Widget Status Tips to StatusBar.
   */
  virtual bool event(QEvent *) override;

  /**
   * @brief Handle unsaved changes ...
   * @see QDialog::isWindowModified
   *
   * Checking if isWindowModified() returning a positive response,
   * on true it will open a QMessageBox with asking for save operation.
   */
  virtual void closeEvent(QCloseEvent *) override;

public:
  /**
   * @param parent - parent widget
   */
  explicit ADialog(QWidget *parent = nullptr);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_DIALOG_H
