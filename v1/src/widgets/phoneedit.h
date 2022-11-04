// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PHONEEDIT_H
#define ANTIQUACRM_PHONEEDIT_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QAbstractListModel>
#include <QCompleter>
#include <QRegExp>
#include <QRegExpValidator>
#include <QRegularExpression>

/**
 * @brief Phone Country Code Completer Model
 * @section widgets
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

class PhoneEdit final : public InputEdit {
  Q_OBJECT

private:
  AntiquaLineEdit *m_edit;
  QCompleter *m_completer;
  QRegExpValidator *m_validator;

  /**
   * @brief Simple Phone Regular expression
   * Containing "ITU-T E.123", "ITU-T E.164" and EPP definition.
   * @code
   *  (Country Code) (NPA Area Code) (Prefix) (Subscriber)
   * @endcode
   */
  static const QRegExp phonePattern();

  /**
   * @brief Validate and Highlight tel: input.
   */
  bool validate(const QString &phone) const;

private Q_SLOTS:
  void dataChanged(const QString &);

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setValue(const QVariant &);
  void setFocus();

public:
  explicit PhoneEdit(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field);
  void loadDataset();
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_PHONEEDIT_H
