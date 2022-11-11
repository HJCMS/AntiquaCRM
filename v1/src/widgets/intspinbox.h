// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_INTSPINBOX_H
#define ANTIQUACRM_INTSPINBOX_H

#include <QObject>
#include <QVariant>
#include <QWidget>
#include <AntiquaInputEdit>

class IntSpinBox final : public InputEdit {
  Q_OBJECT

private:
  int min = 0; /**< Wir verwenden keine Netgativ Werte */
  int max = 9999; /**< Startwert siehe @ref setRange */
  AntiquaSpinBox *m_spinBox;
  void loadDataset();

private Q_SLOTS:
  void itemChanged(int);

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setFocus();
  void setValue(int);

public:
  explicit IntSpinBox(QWidget *parent = nullptr);
  IntSpinBox(int minimum, int maximum, QWidget *parent = nullptr);
  void setProperties(const QSqlField &field);
  void setRange(int minimum, int maximum);
  void setSingleStep(int);
  void setSuffix(const QString &);
  void setPrefix(const QString &);
  void setShowButtons(bool b = true);
  void setValue(const QVariant &);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_INTSPINBOX_H
