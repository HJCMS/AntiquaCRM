// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_NUMEDIT_H
#define ANTIQUACRM_WIDGETS_NUMEDIT_H

#include <AntiquaInput>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class NumEdit
 * @brief Numeric Input Editor
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY NumEdit final : public AntiquaCRM::AInputWidget {
  Q_OBJECT

private:
  ASpinBox *m_edit;

private Q_SLOTS:
  void valueChanged(int);

protected:
  void initData() override;

public Q_SLOTS:
  void setValue(const QVariant &) override;

  void setFocus() override;

  void reset() override;

public:
  /**
   * @param parent - parent widget
   */
  explicit NumEdit(QWidget *parent = nullptr);

  void setReadOnly(bool b = false);

  void setRestrictions(const QSqlField &) override;

  void setRange(int min, int max = 99999);

  void setSingleStep(int step = 0);

  void setPrefix(const QString &);

  void setSuffix(const QString &);

  void setInputToolTip(const QString &) override;

  void setBuddyLabel(const QString &) override;

  bool isValid() override;

  const QMetaType getType() const override;

  const QVariant getValue() override;

  const QString popUpHints() override;

  const QString statusHints() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_NUMEDIT_H
