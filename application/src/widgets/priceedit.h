// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PRICEEDIT_H
#define ANTIQUACRM_PRICEEDIT_H

#include <AntiquaInputEdit>
#include <QLabel>
#include <QObject>
#include <QWidget>

/**
 * @brief The PriceEdit class
 * @ingroup widgets
 */
class PriceEdit final : public InputEdit {
  Q_OBJECT

private:
  AntiquaDoubleBox *m_box;
  void loadDataset() override;

private Q_SLOTS:
  void itemChanged(double);

public Q_SLOTS:
  Q_INVOKABLE void reset() override;
  void setFocus() override;

public:
  explicit PriceEdit(double minimum, double maximum, QWidget *parent = nullptr);
  explicit PriceEdit(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field) override;
  void setMinimum(double);
  void setMaximum(double);
  void setRange(double min, double max);
  void setValue(const QVariant &) override;
  const QVariant value() override;
  bool isValid() override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QString notes() override;
};

#endif // ANTIQUACRM_PRICEEDIT_H
