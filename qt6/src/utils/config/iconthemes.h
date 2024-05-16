// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_DIALOGS_ICONTHEMES_H
#define ANTIQUACRM_DIALOGS_ICONTHEMES_H

#include <AntiquaInput>
#include <QIcon>
#include <QFrame>

class IconThemes final : public AntiquaCRM::AInputWidget, private QIcon {
  Q_OBJECT

private:
  AntiquaCRM::AComboBox *m_edit;

  const QString getName(const QString &path) const;

private Q_SLOTS:
  void valueChanged(int);

public Q_SLOTS:
  void setValue(const QVariant &) override;
  void setFocus() override;
  void reset() override;

public:
  explicit IconThemes(QWidget *parent = nullptr);
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

#endif // ANTIQUACRM_DIALOGS_ICONTHEMES_H
