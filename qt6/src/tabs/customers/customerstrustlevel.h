// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_CUSTOMERSTRUSTLEVEL_H
#define ANTIQUACRM_PLUGIN_CUSTOMERSTRUSTLEVEL_H

#include <AntiquaWidgets>
#include <QComboBox>
#include <QObject>
#include <QWidget>

/**
 * @brief Customers Trustlevel editor
 * @ingroup _customers
 */
class CustomersTrustLevel final : public AntiquaCRM::AInputWidget {
  Q_OBJECT

private:
  AntiquaCRM::AComboBox* m_edit;

private Q_SLOTS:
  void valueChanged(int);

protected:
  void initData() override;

public Q_SLOTS:
  void setValue(const QVariant&) override;
  void setFocus() override;
  void reset() override;

public:
  explicit CustomersTrustLevel(QWidget* parent = nullptr);
  void setRestrictions(const QSqlField&) override;
  void setInputToolTip(const QString&) override;
  void setBuddyLabel(const QString&) override;
  bool isValid() override;
  const QMetaType getType() const override;
  const QVariant getValue() override;
  const QString popUpHints() override;
  const QString statusHints() override;
};

#endif // ANTIQUACRM_PLUGIN_CUSTOMERSTRUSTLEVEL_H
