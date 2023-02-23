// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PRINTING_LAYOUT_H
#define PRINTING_LAYOUT_H

#include <AntiquaInputEdit>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QMarginsF>
#include <QObject>
#include <QSlider>
#include <QWidget>

class BorderPrintInput final : public InputEdit {
  Q_OBJECT

private:
  QDoubleSpinBox *m_box;
  QSlider *m_slider;
  void loadDataset() override{/* unused */};

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

class PrintingLayout final : public QGroupBox {
  Q_OBJECT

private:
  BorderPrintInput *m_left;
  BorderPrintInput *m_right;
  qreal p_top, p_bottom;

  QLabel *label(const QString &);

public Q_SLOTS:
  void setValue(const QMarginsF &);
  void setValue(qreal, qreal, qreal, qreal);

public:
  explicit PrintingLayout(QWidget *parent = nullptr);
  const QMarginsF value();
};

#endif // PRINTING_LAYOUT_H
