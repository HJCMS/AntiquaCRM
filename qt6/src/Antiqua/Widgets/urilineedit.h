// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_URI_LINEEDIT_H
#define ANTIQUACRM_WIDGETS_URI_LINEEDIT_H

#include <AntiquaInput>
#include <QAction>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class URILineEdit
 * @brief Uniform Resource Identifier input Editor
 *
 * Currently: eMail and Weblink supported
 *
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY URILineEdit final : public AntiquaCRM::AInputWidget {
  Q_OBJECT

private:
  AntiquaCRM::ALineEdit *m_edit;
  QAction *ac_icon;

  /**
   * @brief URI validation
   */
  bool validate(const QString &uri) const;

  void initData() override;

private Q_SLOTS:
  void valueChanged(const QString &uri);
  void openWeblink();

public Q_SLOTS:
  void setValue(const QVariant &) override;

  void setFocus() override;

  void reset() override;

public:
  /**
   * @param name   - Object name
   * @param parent - parent Object
   */
  explicit URILineEdit(const QString &name, QWidget *parent = nullptr);
  explicit URILineEdit(QWidget *parent = nullptr);

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

#endif // ANTIQUACRM_WIDGETS_URI_LINEEDIT_H
