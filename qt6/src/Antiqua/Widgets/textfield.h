// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_TEXTFIELD_H
#define ANTIQUACRM_WIDGETS_TEXTFIELD_H

#include <AntiquaInput>
#include <QTextEdit>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @ingroup AntiquaWidgets
 * @class TextField
 */
class ANTIQUACRM_LIBRARY TextField final : public AntiquaCRM::AbstractInput {
  Q_OBJECT

private:
  QTextEdit *m_edit;
  const QString stripString(const QString &) const;

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
   * @brief __TODO__
   * @param parent - parent widget
   */
  explicit TextField(QWidget *parent = nullptr);

  void setRestrictions(const QSqlField &) override;

  void setInputToolTip(const QString &) override;

  void setBuddyLabel(const QString &) override;

  bool isValid() override;

  const QVariant getValue() override;

  const QString popUpHints() override;

  const QString statusHints() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_TEXTFIELD_H
