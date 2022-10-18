// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SIZEEDIT_H
#define ANTIQUACRM_SIZEEDIT_H

#include <AntiquaInputEdit>
#include <QObject>
#include <QSpinBox>

class SizeEdit final : public InputEdit {
  Q_OBJECT

private:
  QSpinBox *m_width;
  QSpinBox *m_height;
  void loadDataset();

private Q_SLOTS:
  void sizeChanged(int);

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setFocus();
  void setValue(const QVariant &);

public:
  SizeEdit(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field);
  const QVariant value();
  void setMinimum(int width, int height);
  void setMaximum(int width, int height);
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_SIZEEDIT_H
