// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_YEARSELECT_H
#define ANTIQUACRM_YEARSELECT_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QDate>
#include <QPushButton>

class YearSelect final : public InputEdit {
  Q_OBJECT

private:
  int factor = 2;
  QDate p_curDate;
  AntiquaComboBox *m_box;
  QPushButton *btn_open;
  void loadDataset();

private Q_SLOTS:
  void dataChanged(int);

Q_SIGNALS:
  void sendYearOpen();

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setValue(const QVariant &);
  void setFocus();

public:
  explicit YearSelect(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_YEARSELECT_H
