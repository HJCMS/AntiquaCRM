// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_BOOLBOX_H
#define ANTIQUACRM_BOOLBOX_H

#include <AntiquaInputEdit>
#include <QCheckBox>
#include <QObject>
#include <QWidget>

class BoolBox final : public InputEdit {
  Q_OBJECT

private:
  QCheckBox *m_checkBox;
  void loadDataset();

private Q_SLOTS:
  void itemChanged(int);

Q_SIGNALS:
  void checked(bool);

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setFocus();
  void setChecked(bool);
  void setValue(const QVariant &);

public:
  explicit BoolBox(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field);
  bool isChecked();
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_BOOLBOX_H
