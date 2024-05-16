// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_DIALOGS_SSLCASELECTER_H
#define ANTIQUACRM_DIALOGS_SSLCASELECTER_H

#include <AntiquaInput>
#include <QObject>
#include <QLineEdit>
#include <QPushButton>
#include <QSslCertificate>
#include <QRegularExpression>

class SslCaSelecter final : public AntiquaCRM::AInputWidget {
  Q_OBJECT

private:
  QString p_ca_bundle;
  AntiquaCRM::AComboBox *m_edit;
  QPushButton *m_open;
  QLineEdit *m_search;
  static const QRegularExpression pattern();
  void updateBundle(const QString &path);

private Q_SLOTS:
  void valueChanged(int);
  void setSearch(const QString &);
  void setBundleFile();

public Q_SLOTS:
  void setValue(const QVariant &) override;
  void setFocus() override;
  void reset() override;

public:
  explicit SslCaSelecter(QWidget *parent = nullptr);
  void setBundle(const QString &path);
  const QString getBundle();
  const QSslCertificate getCert();
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

#endif // ANTIQUACRM_DIALOGS_SSLCASELECTER_H
