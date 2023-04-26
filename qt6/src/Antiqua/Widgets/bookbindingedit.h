// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_BOOKBINDINGEDIT_H
#define ANTIQUACRM_WIDGETS_BOOKBINDINGEDIT_H

#include <AntiquaInput>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class BookBindingEdit
 * @brief Edit Cover and Bookbinding description
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY BookBindingEdit final
    : public AntiquaCRM::AbstractInput {
  Q_OBJECT

private:
  AComboBox *m_edit;

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
  explicit BookBindingEdit(QWidget *parent = nullptr);

  void initData() override;

  void setRestrictions(const QSqlField &) override;

  void setInputToolTip(const QString &) override;

  void setBuddyLabel(const QString &) override;

  bool isValid() override;

  const QMetaType getType() const override;

  const QVariant getValue() override;

  const QString popUpHints() override;

  const QString statusHints() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_BookBindingEdit_H
