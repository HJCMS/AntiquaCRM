// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_GROUPBOXEDIT_H
#define ANTIQUACRM_WIDGETS_GROUPBOXEDIT_H

#include <AntiquaInput>
#include <QGroupBox>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class GroupBoxEdit
 * @brief The „GroupBoxEdit“ input edit class.
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY GroupBoxEdit final : public AntiquaCRM::AInputWidget {
  Q_OBJECT

private:
  QGroupBox *m_edit;
  QBoxLayout *m_layout;

private Q_SLOTS:
  /**
   * @brief Internal changes when data has changed
   */
  void valueChanged(bool);

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
  explicit GroupBoxEdit(QWidget *parent = nullptr);

  /**
   * @brief Layout for GroupBox content!
   */
  QBoxLayout *boxLayout();

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
   * @brief Current MetaType for this input
   */
  const QMetaType getType() const override;

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
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_GROUPBOXEDIT_H
