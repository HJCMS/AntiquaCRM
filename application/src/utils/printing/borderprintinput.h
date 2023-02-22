// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PRINTING_BORDER_INPUT_H
#define PRINTING_BORDER_INPUT_H

#include <QObject>
#include <QWidget>
#include <AntiquaInputEdit>
#include <QDoubleSpinBox>
#include <QSlider>

class BorderPrintInput final : public InputEdit {
  Q_OBJECT

private:
  QDoubleSpinBox *m_box;
  QSlider *m_slider;
  void loadDataset() override;

private Q_SLOTS:
  void borderChanged(int);

public Q_SLOTS:
  Q_INVOKABLE void reset() override;
  void setFocus() override;
  void setValue(const QVariant &) override;

public:
  explicit BorderPrintInput(QWidget *parent = nullptr);
  void setProperties(const QSqlField &) override;
  const QVariant value() override;
  bool isValid() override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QString notes() override;
};

#endif // PRINTING_BORDER_INPUT_H
