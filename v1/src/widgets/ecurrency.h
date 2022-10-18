// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ECURRENCY_H
#define ANTIQUACRM_ECURRENCY_H

#include <AntiquaInputEdit>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QObject>

class Ecurrency final : public InputEdit {
  Q_OBJECT

private:
  QLineEdit *m_lineEdit;
  QComboBox *m_comboBox;
  void loadDataset();

private Q_SLOTS:
  void itemChanged(int);

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setFocus();
  void setValue(const QVariant &);

public:
  explicit Ecurrency(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ECURRENCY_UTILS_H
