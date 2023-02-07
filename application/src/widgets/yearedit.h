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
  void loadDataset() override;

private Q_SLOTS:
  void dataChanged(const QDate &);

public Q_SLOTS:
  void setValue(const QVariant &) override;
  Q_INVOKABLE void reset() override;
  void setFocus() override;

public:
  explicit YearEdit(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field) override;
  const QVariant value() override;
  bool isValid() override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QString notes() override;
};

#endif // ANTIQUACRM_YEAREDIT_H
