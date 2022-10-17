// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_YEAREDIT_H
#define ANTIQUACRM_YEAREDIT_H

#include <AntiquaInputEdit>
#include <QDateEdit>
#include <QObject>
#include <QWidget>

class YearEdit final : public InputEdit {
  Q_OBJECT

private:
  QDateEdit *m_year;
  const QDate startDate;
  void loadDataset();

private Q_SLOTS:
  void dataChanged(const QDate &);

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();
  void setFocus();

public:
  explicit YearEdit(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_YEAREDIT_H
