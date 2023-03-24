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
  void valueChanged(int);

protected:
  virtual void initData() override;

public Q_SLOTS:
  virtual void setValue(const QVariant &) override;

  virtual void setFocus() override;

  virtual void reset() override;

public:
  /**
   * @brief Salutation selecter class
   * @param parent - parent widget
   */
  explicit SalutationEdit(QWidget *parent = nullptr);

  virtual void setRestrictions(const QSqlField &) override;

  virtual void setInputToolTip(const QString &) override;

  virtual void setBuddyLabel(const QString &) override;

  virtual bool isValid() override;

  virtual const QVariant getValue() override;

  virtual const QString popUpHints() override;

  virtual const QString statusHints() override;

  /**
   * @brief get all salutations from json document
   */
  static const QMap<QString, QString> salutations();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_SALUTATIONEDIT_H
