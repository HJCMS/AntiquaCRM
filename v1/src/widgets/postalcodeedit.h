// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_POSTALCODE_EDIT_H
#define ANTIQUACRM_POSTALCODE_EDIT_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QCompleter>
#include <QLineEdit>
#include <QAbstractListModel>

/**
 * @brief Postalcode Completer Model
 * @section widgets
 */
class PostalCodeModel final : public QAbstractListModel {
  Q_OBJECT

private:
  QList<AntiquaCRM::PostalCode> p_codes;

public:
  explicit PostalCodeModel(QObject *parent = nullptr);
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
  void initModel(const QString &country);
};

/**
 * @brief Postalcode selection Widget
 * @section widgets
 */
class PostalCodeEdit final : public InputEdit {
  Q_OBJECT

private:
  /**
   * @brief Available country postalcode sets
   */
  AntiquaComboBox *m_countries;

  /**
   * @brief Search edit postalcode for selected country
   */
  QLineEdit *m_postalcode;

  /**
   * @brief completer for selected country
   */
  QCompleter *m_completer;

private Q_SLOTS:
  void dataChanged(int);
  void postalReadyToLeave();

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setValue(const QVariant &val);
  void setFocus();
  void findCountry(const QString &name);

Q_SIGNALS:
  void sendOnLeavePostalEdit(const AntiquaCRM::PostalCode &);

public:
  explicit PostalCodeEdit(QWidget *parent = nullptr);
  void loadDataset();
  void setProperties(const QSqlField &field);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString country();
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_POSTALCODE_EDIT_H
