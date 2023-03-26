// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_EMAILEDIT_H
#define ANTIQUACRM_WIDGETS_EMAILEDIT_H

#include <AntiquaInput>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @ingroup AntiquaWidgets
 * @class EMailEdit
 */
class ANTIQUACRM_LIBRARY EMailEdit final : public AntiquaCRM::AbstractInput {
  Q_OBJECT

private:
  AntiquaCRM::ALineEdit *m_edit;

  /**
   * @brief eMail validation
   */
  bool validate(const QString &) const;

  void initData() override;

private Q_SLOTS:
  void valueChanged(const QString &);

public Q_SLOTS:
  void setValue(const QVariant &) override;

  void setFocus() override;

  void reset() override;

public:
  /**
   * @brief eMail input Editor
   * @param name   - Object name
   * @param parent - parent Object
   */
  explicit EMailEdit(const QString &name, QWidget *parent = nullptr);
  explicit EMailEdit(QWidget *parent = nullptr);

  void setRestrictions(const QSqlField &) override;

  void setInputToolTip(const QString &) override;

  void setBuddyLabel(const QString &) override;

  bool isValid() override;

  const QVariant getValue() override;

  const QString popUpHints() override;

  const QString statusHints() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_EMAILEDIT_H
