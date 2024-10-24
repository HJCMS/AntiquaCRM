// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
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
 * @class SalutationEdit
 * @brief Salutation selecter class
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY SalutationEdit final
    : public AntiquaCRM::AInputWidget {
  Q_OBJECT

private:
  AntiquaCRM::AComboBox *m_edit;
  AntiquaCRM::ALineEdit *m_lineEdit;

private Q_SLOTS:
  void valueChanged(int);

public Q_SLOTS:
  void setValue(const QVariant &) override;

  void setFocus() override;

  void reset() override;

public:
  /**
   * @param parent - parent widget
   */
  explicit SalutationEdit(QWidget *parent = nullptr);

  /**
   * @brief Loading Data from Json file.
   */
  void initData() override;

  void setRestrictions(const QSqlField &) override;

  void setInputToolTip(const QString &) override;

  void setBuddyLabel(const QString &) override;

  bool isValid() override;

  const QMetaType getType() const override;

  const QVariant getValue() override;

  const QString popUpHints() override;

  const QString statusHints() override;

  /**
   * @brief get all salutations from json document
   * @param bcp47 - Default fallback if no System language supported!
   */
  static const QMap<QString, QString> map(const QString &bcp47 = QString("en"));
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_SALUTATIONEDIT_H
