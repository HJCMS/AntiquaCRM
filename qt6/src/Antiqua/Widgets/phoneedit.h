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
 * @ingroup AntiquaWidgets
 * @class PhoneCountryCodeModel
 */
class PhoneCountryCodeModel final : public QAbstractListModel {
  Q_OBJECT

private:
  struct CountryCode {
    QString npa;
    QString info;
  };
  QList<CountryCode> p_codes;

public:
  /**
   * @brief Phone Country Code Completer Model
   */
  explicit PhoneCountryCodeModel(QObject *parent = nullptr);
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
  void initModel();
};

/**
 * @ingroup AntiquaWidgets
 * @class PhoneEdit
 */
class ANTIQUACRM_LIBRARY PhoneEdit final : public AntiquaCRM::AbstractInput {
  Q_OBJECT

private:
  AntiquaCRM::ALineEdit *m_edit;
  QRegularExpressionValidator *m_validator;
  QCompleter *m_completer;

  bool validate(const QString &) const;

  virtual void initData() override;

private Q_SLOTS:
  /**
   * @brief price has been changed
   */
  void valueChanged(const QString &);

public Q_SLOTS:
  /**
   * @brief set eMail
   */
  virtual void setValue(const QVariant &) override;

  /**
   * @brief set input focus
   */
  virtual void setFocus() override;

  /**
   * @brief clear input and reset window modified
   */
  virtual void reset() override;

public:
  /**
   * @brief PhoneEdit
   * @param name   - Object name
   * @param parent -
   */
  explicit PhoneEdit(const QString &name, QWidget *parent = nullptr);
  explicit PhoneEdit(QWidget *parent = nullptr);

  virtual void setRestrictions(const QSqlField &) override;

  virtual void setInputToolTip(const QString &) override;

  virtual void setBuddyLabel(const QString &) override;

  virtual bool isValid() override;

  virtual const QVariant getValue() override;

  virtual const QString popUpHints() override;

  virtual const QString statusHints() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_PHONEEDIT_H