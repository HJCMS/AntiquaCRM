// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_BOOLBOX_H
#define ANTIQUACRM_BOOLBOX_H

#include <AntiquaInputEdit>
#include <QCheckBox>
#include <QObject>
#include <QWidget>

/**
 * @brief The BoolBox class
 * @ingroup widgets
 */
class BoolBox final : public InputEdit {
  Q_OBJECT

private:
  QCheckBox *m_checkBox;
  void loadDataset() override;

private Q_SLOTS:
  void itemChanged(int);

Q_SIGNALS:
  void checked(bool);

public Q_SLOTS:
  Q_INVOKABLE void reset() override;
  void setFocus() override;
  void setChecked(bool);
  void setValue(const QVariant &) override;

public:
  explicit BoolBox(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field) override;
  bool isChecked();
  const QVariant value() override;
  bool isValid() override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QString notes() override;
};

#endif // ANTIQUACRM_BOOLBOX_H
