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

  /**
   * @brief Simple File Basename match
   * @param baseName file basename
   *
   * Simplified file basename match to reduce buffercosumption by using regular expressions.
   */
  inline bool simpleMatch(const QString &baseName) const;

  /**
   * @brief Regular Expression pattern for ca-bundle
   *
   * Pattern for matching file base names without extension.
   * @code
   *  '^(ca[\\-_])(bundle|certificate)([\\-_\\.]trust)?$'
   * @endcode
   */
  static const QRegularExpression pattern();

  /**
   * @brief updateBundle
   * @param path File Path
   *
   * Updates the Ssl configuration when bundle file was changed.
   */
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
