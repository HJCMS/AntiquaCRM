// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_STORAGELOCATION_H
#define ANTIQUACRM_STORAGELOCATION_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QLineEdit>

class StorageLocation final : public InputEdit {
  Q_OBJECT

private:
  AntiquaComboBox *m_box;
  QLineEdit *m_search;

private Q_SLOTS:
  void dataChanged(int);
  void filterChanged(const QString &);

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setValue(const QVariant &);
  void setFocus();

public:
  explicit StorageLocation(QWidget *parent = nullptr);
  void loadDataset();
  void setProperties(const QSqlField &field);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_STORAGELOCATION_H
