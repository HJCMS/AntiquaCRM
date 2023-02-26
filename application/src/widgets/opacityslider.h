// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_OPACITY_SLIDER_H
#define ANTIQUACRM_OPACITY_SLIDER_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QSlider>

class OpacitySlider final : public InputEdit {
  Q_OBJECT

private:
  QSlider *m_slider;
  QLabel *lb_info;
  void loadDataset(){/*unused*/};

private Q_SLOTS:
  void dataChanged(int);

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setValue(const QVariant &);
  void setFocus();

public:
  explicit OpacitySlider(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_OPACITY_SLIDER_H
