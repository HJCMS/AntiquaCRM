// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_POSTALCODEEDIT_H
#define ANTIQUACRM_POSTALCODEEDIT_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QComboBox>
#include <QCompleter>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>
#include <QAbstractListModel>

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

class PostalCodeEdit final : public InputEdit {
  Q_OBJECT

private:
  /**
   * @brief info label
   */
  QLabel *m_label;

  /**
   * @brief Available country postalcode sets
   */
  QComboBox *m_countries;

  /**
   * @brief Search edit postalcode for selected country
   */
  QLineEdit *m_postalcode;

  /**
   * @brief completer for selected country
   */
  QCompleter *m_completer;

  /**
   * @brief load countries in to m_countries
   */
  void loadDataset();

private Q_SLOTS:
  void dataChanged(int);
  void postalReadyToLeave();

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setValue(const QVariant &);
  void setFocus();

Q_SIGNALS:
  void sendOnLeavePostalEdit(const AntiquaCRM::PostalCode &);

public:
  explicit PostalCodeEdit(QWidget *parent = nullptr);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_POSTALCODEEDIT_H
