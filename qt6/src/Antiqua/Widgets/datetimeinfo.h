// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_DATETIMEINFO_H
#define ANTIQUACRM_WIDGETS_DATETIMEINFO_H

#include <AntiquaInput>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class DateTimeInfo
 * @brief Show Date time Information (no editor)
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY DateTimeInfo final : public AntiquaCRM::AbstractInput {
  Q_OBJECT

private:
  ADateTimeEdit *m_edit;

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
  explicit DateTimeInfo(QWidget *parent = nullptr);

  void setRestrictions(const QSqlField &) override;

  void setInputToolTip(const QString &) override;

  void setBuddyLabel(const QString &) override;

  bool isValid() override;

  const QVariant getValue() override;

  const QString popUpHints() override;

  const QString statusHints() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_DATETIMEINFO_H
