// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_DIALOGS_SSLMODE_H
#define ANTIQUACRM_DIALOGS_SSLMODE_H

#include <AntiquaInput>
#include <QObject>

class SslMode final : public AntiquaCRM::AInputWidget {
  Q_OBJECT

private:
  AntiquaCRM::AComboBox *m_edit;
  void initData() override;

private Q_SLOTS:
  void valueChanged(int);

public Q_SLOTS:
  void setValue(const QVariant &) override;
  void setFocus() override;
  void reset() override;

public:
  explicit SslMode(QWidget *parent = nullptr);
  void setRestrictions(const QSqlField &) override;
  void setInputToolTip(const QString &) override;
  void setBuddyLabel(const QString &) override;
  bool isValid() override;
  const QMetaType getType() const override;
  const QVariant getValue() override;
  const QString popUpHints() override;
  const QString statusHints() override;
};

#endif // ANTIQUACRM_DIALOGS_SSLMODE_H
