// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_PHONEEDIT_H
#define ANTIQUACRM_PHONEEDIT_H

#include <AntiquaInputEdit>
#include <QAbstractListModel>
#include <QCompleter>
#include <QRegExpValidator>

/**
 * @brief Phone Country Code Completer Model
 * @ingroup widgets
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
  explicit PhoneCountryCodeModel(QObject *parent = nullptr);
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
  void initModel();
};

/**
 * @brief The PhoneEdit class
 * @ingroup widgets
 */
class PhoneEdit final : public InputEdit {
  Q_OBJECT

private:
  AntiquaLineEdit *m_edit;
  QCompleter *m_completer;
  QRegExpValidator *m_validator;

  /**
   * @brief Validate and Highlight tel: input.
   */
  bool validate(const QString &phone) const;

private Q_SLOTS:
  void dataChanged(const QString &);

public Q_SLOTS:
  Q_INVOKABLE void reset() override;
  void setValue(const QVariant &) override;
  void setFocus() override;

public:
  explicit PhoneEdit(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field) override;
  void loadDataset() override;
  const QVariant value() override;
  bool isValid() override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QString notes() override;
};

#endif // ANTIQUACRM_PHONEEDIT_H
