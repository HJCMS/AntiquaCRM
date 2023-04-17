// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_MAILBUTTON_H
#define ANTIQUACRM_WIDGETS_MAILBUTTON_H

#include <AGlobal>
#include <QActionGroup>
#include <QMenu>
#include <QObject>
#include <QPushButton>

namespace AntiquaCRM {

/**
 * @class MailButton
 * @brief The MailButton class
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY MailButton final : public QPushButton {
  Q_OBJECT

private:
  MailGroups buttons;
  QMenu *m_menu;
  bool createMailButtonActions();

private Q_SLOTS:
  void setMailAction();

Q_SIGNALS:
  /**
   * @brief Signal changes in the flags
   */
  void sendSectionChanged();

  /**
   * @brief Send email message from constant...
   * @see createMailButtonActions()
   */
  void sendMailAction(const QString &);

public Q_SLOTS:
  /**
   * @brief Only show if an email exists!
   */
  void hasMailAddress(bool b = false);

public:
  explicit MailButton(QWidget *parent = nullptr);

  /**
   * @brief Display settings
   */
  void setSections(AntiquaCRM::MailGroups);

  /**
   * @brief Which flags are set?
   */
  AntiquaCRM::MailGroups getSections();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_MAILBUTTON_H
