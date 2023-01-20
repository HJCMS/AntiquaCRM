// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_BOOKBINDING_H
#define ANTIQUACRM_BOOKBINDING_H

#include <QWidget>
#include <AntiquaInputEdit>

class BookBinding : public InputEdit {
  Q_OBJECT

private:
  AntiquaComboBox *m_box;

private Q_SLOTS:
  void dataChanged(int);

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setValue(const QVariant &);
  void setFocus();

public:
  explicit BookBinding(QWidget *parent = nullptr);
  void loadDataset();
  void setProperties(const QSqlField &field);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_BOOKBINDING_H