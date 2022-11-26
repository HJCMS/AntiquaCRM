// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_DATEINFO_H
#define ANTIQUACRM_DATEINFO_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QDateTimeEdit>

class AntiquaDateInfo final : public InputEdit {
  Q_OBJECT

private:
  QDateTimeEdit *m_edit;
  void loadDataset();

private Q_SLOTS:
  void dateChanged();

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setValue(const QVariant &);
  void setFocus();

public:
  explicit AntiquaDateInfo(QWidget *parent = nullptr);
  void setDisplayFormat(const QString &format);
  void setProperties(const QSqlField &field);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_DATEINFO_H
