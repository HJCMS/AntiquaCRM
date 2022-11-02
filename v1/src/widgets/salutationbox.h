// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SALUTATIONBOX_H
#define ANTIQUACRM_SALUTATIONBOX_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QLineEdit>
#include <QObject>
#include <QWidget>

class SalutationBox final : public InputEdit {
  Q_OBJECT

private:
  AntiquaComboBox *m_box;
  QLineEdit *m_edit;
  void loadDataset();

private Q_SLOTS:
  void dataChanged(int);

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setValue(const QVariant &);
  void setFocus();

public:
  explicit SalutationBox(QWidget *parent = nullptr);
  static const QString withoutDisclosures();
  static const QStringList salutations();
  void setProperties(const QSqlField &field);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_SALUTATIONBOX_H
