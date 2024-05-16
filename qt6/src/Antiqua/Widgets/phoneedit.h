// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
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
 *
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY PhoneCountryCodeModel final
    : public QAbstractListModel {
  Q_OBJECT

private:
  /**
   * @brief item data class
   */
  struct CountryCode {
    QString npa;  /**< national phone area */
    QString info; /**< location info */
  };

  /**
   * @brief international phone items list
   */
  QList<CountryCode> p_codes;

public:
  /**
   * @param parent - parent object
   */
  explicit PhoneCountryCodeModel(QObject *parent = nullptr);

  /**
   * @brief get item count
   * @param parent - ModelIndex (unused)
   */
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  /**
   * @brief columnCount - always=2
   * @param parent - ModelIndex (unused)
   */
  int columnCount(const QModelIndex &parent) const override;

  /**
   * @brief get CountryCode with ModelIndex
   * @param index - ModelIndex
   * @param role - Only (DisplayRole | EditRole | ToolTipRole) supported!
   */
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

  /**
   * @brief Header column data
   * @note this model have no Header data
   * @param section - (unused)
   * @param orientation - (unused)
   * @param role - (unused)
   * @return always null
   */
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;

  /**
   * @brief Search for Json database and read it ...
   */
  void initModel();
};

/**
 * @class PhoneEdit
 * @brief Phone and Mobil number input edit
 *
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY PhoneEdit final : public AntiquaCRM::AInputWidget {
  Q_OBJECT

private:
  AntiquaCRM::ALineEdit *m_edit;

  /**
   * @brief Validation with Phone pattern.
   * @sa AUtil::phoneRegExp
   */
  QRegularExpressionValidator *m_validator;

  /**
   * @brief International Area codes completer
   * @sa initData
   */
  QCompleter *m_completer;

  /**
   * @brief „DIN 5008/E.123“ phone number validation
   */
  bool validate(const QString &) const;

  /**
   * @brief Set restrictions and create Completer data.
   * @sa PhoneCountryCodeModel::initModel()
   */
  void initData() override;

private Q_SLOTS:
  /**
   * @brief Used by QLineEdit::textChanged
   *
   * This slot is used for validation and visual errors.
   */
  void valueChanged(const QString &);

public Q_SLOTS:
  /**
   * @brief Insert Phone Number
   * @brief value - numeric string
   * @note We didn't supporting plus sign in Phone Numbers!
   *
   * Only a numeric string accepted and a leading \b + will replaced by \b 0.
   * This is not DIN 5008/E.123 conform but faster to find and validate.
   */
  void setValue(const QVariant &) override;

  void setFocus() override;
  void reset() override;

public:
  /**
   * @param parent - parent object
   * @param name   - important object name from database fields
   */
  explicit PhoneEdit(QWidget *parent = nullptr,
                     const QString &name = QString("phone_edit"));

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
