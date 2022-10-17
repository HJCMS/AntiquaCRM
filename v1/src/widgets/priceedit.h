// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PRICEEDIT_H
#define ANTIQUACRM_PRICEEDIT_H

#include <AntiquaInputEdit>
#include <QLabel>
#include <QObject>
#include <QWidget>

class PriceEdit final : public InputEdit {
  Q_OBJECT

private:
  AntiquaDoubleBox *m_box;
  void loadDataset();

private Q_SLOTS:
  void itemChanged(double);

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setFocus();

public:
  explicit PriceEdit(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field);
  void setMinimum(double);
  void setValue(const QVariant &);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_PRICEEDIT_H
