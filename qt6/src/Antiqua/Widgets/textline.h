// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_TEXTLINE_H
#define ANTIQUACRM_WIDGETS_TEXTLINE_H

#include <AntiquaInput>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class TextLine
 * @brief Single Text input with LineEdit
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY TextLine final : public AntiquaCRM::AInputWidget {
  Q_OBJECT

private:
  ALineEdit *m_edit;

private Q_SLOTS:
  void valueChanged(const QString &);

protected:
  void initData() override;

Q_SIGNALS:
  void signalEnterPressed();

public Q_SLOTS:
  void setValue(const QVariant &) override;

  void setFocus() override;

  void reset() override;

public:
  /**
   * @param parent - parent widget
   */
  explicit TextLine(QWidget *parent = nullptr);

  void setReadOnly(bool b);

  /**
   * @brief Add QStringList and create a QCompleter from it
   */
  void setCompleterList(const QStringList &);

  void setPasswordInput(bool b = false);

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

#endif // ANTIQUACRM_WIDGETS_TEXTLINE_H
