// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_INTSPINBOX_H
#define ANTIQUACRM_INTSPINBOX_H

#include <AntiquaInputEdit>

/**
 * @brief The IntSpinBox class
 * @ingroup widgets
 */
class IntSpinBox final : public InputEdit {
  Q_OBJECT

private:
  AntiquaSpinBox *m_box;
  void loadDataset() override{};

private Q_SLOTS:
  void itemChanged(int);

public Q_SLOTS:
  Q_INVOKABLE void reset() override;
  void setFocus() override;
  void setValue(const QVariant &) override;
  Q_DECL_DEPRECATED void setValue(int i) { setValue(QVariant(i)); };

public:
  explicit IntSpinBox(int minimum, int maximum, QWidget *parent = nullptr);
  explicit IntSpinBox(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field) override;
  void setRange(int minimum, int maximum);
  void setSingleStep(int);
  void setSuffix(const QString &);
  void setPrefix(const QString &);
  void setShowButtons(bool b = true);
  const QVariant value() override;
  bool isValid() override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QString notes() override;
};

#endif // ANTIQUACRM_INTSPINBOX_H
