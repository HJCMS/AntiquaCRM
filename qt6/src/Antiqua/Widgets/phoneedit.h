// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_PHONEEDIT_H
#define ANTIQUACRM_WIDGETS_PHONEEDIT_H

#include <AntiquaInput>
#include <QAbstractListModel>
#include <QCompleter>
#include <QList>
#include <QObject>
#include <QRegularExpressionValidator>
#include <QVariant>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class PhoneCountryCodeModel
 * @brief Phone Country Code Completer Model
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY PhoneCountryCodeModel final
    : public QAbstractListModel {
  Q_OBJECT

private:
  struct CountryCode {
    QString npa;
    QString info;
  };
  QList<CountryCode> p_codes;

public:
  /**
   * @param parent - parent object
   */
  explicit PhoneCountryCodeModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  /**
   * @brief columnCount - always=2
   * @param parent - ModelIndex
   */
  int columnCount(const QModelIndex &parent) const override;

  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;

  void initModel();
};

/**
 * @class PhoneEdit
 * @brief Phone/Mobil number input edit
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY PhoneEdit final : public AntiquaCRM::AInputWidget {
  Q_OBJECT

private:
  AntiquaCRM::ALineEdit *m_edit;
  QRegularExpressionValidator *m_validator;
  QCompleter *m_completer;

  /**
   * @brief „DIN 5008/E.123“ phone number validation
   */
  bool validate(const QString &) const;

  void initData() override;

private Q_SLOTS:
  void valueChanged(const QString &);

public Q_SLOTS:
  void setValue(const QVariant &) override;

  void setFocus() override;

  void reset() override;

public:
  /**
   * @param name   - Object name
   * @param parent - parent object
   */
  explicit PhoneEdit(const QString &name, QWidget *parent = nullptr);
  explicit PhoneEdit(QWidget *parent = nullptr);

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

#endif // ANTIQUACRM_WIDGETS_PHONEEDIT_H
