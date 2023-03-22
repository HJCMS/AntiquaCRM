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

  bool validate(const QString &) const;

  virtual void initData() override;

private Q_SLOTS:
  /**
   * @brief price has been changed
   */
  void valueChanged(const QString &);

public Q_SLOTS:
  /**
   * @brief set eMail
   */
  virtual void setValue(const QVariant &) override;

  /**
   * @brief set input focus
   */
  virtual void setFocus() override;

  /**
   * @brief clear input and reset window modified
   */
  virtual void reset() override;

public:
  /**
   * @brief EMailEdit
   * @param name   - Object name
   * @param parent -
   */
  explicit EMailEdit(const QString &name, QWidget *parent = nullptr);
  explicit EMailEdit(QWidget *parent = nullptr);

  virtual void setRestrictions(const QSqlField &) override;

  virtual void setInputToolTip(const QString &) override;

  virtual void setBuddyLabel(const QString &) override;

  virtual bool isValid() override;

  virtual const QVariant getValue() override;

  virtual const QString popUpHints() override;

  virtual const QString statusHints() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_EMAILEDIT_H
