// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_BOOLBOX_H
#define ANTIQUACRM_WIDGETS_BOOLBOX_H

#include <AntiquaInput>
#include <QCheckBox>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class BoolBox
 * @brief Antiqua Checkbox
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY BoolBox final : public AntiquaCRM::AbstractInput {
  Q_OBJECT

private:
  QCheckBox *m_edit;

private Q_SLOTS:
  void valueChanged();

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
  explicit BoolBox(QWidget *parent = nullptr);

  void setRestrictions(const QSqlField &) override;

  void setInputToolTip(const QString &) override;

  void setBuddyLabel(const QString &) override;

  bool isValid() override;

  const QVariant getValue() override;

  const QString popUpHints() override;

  const QString statusHints() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_BOOLBOX_H
