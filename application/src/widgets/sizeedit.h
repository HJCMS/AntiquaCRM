// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SIZEEDIT_H
#define ANTIQUACRM_SIZEEDIT_H

#include <AntiquaInputEdit>
#include <QObject>
#include <QSpinBox>

/**
 * @brief Size edit input widget
 * @ingroup widgets
 */
class SizeEdit final : public InputEdit {
  Q_OBJECT

private:
  QSpinBox *m_width;
  QSpinBox *m_height;
  void loadDataset() override;

private Q_SLOTS:
  void sizeChanged(int);

public Q_SLOTS:
  Q_INVOKABLE void reset() override;
  void setFocus() override;
  void setValue(const QVariant &) override;

public:
  SizeEdit(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field) override;
  const QVariant value() override;
  void setMinimum(int width, int height);
  void setMaximum(int width, int height);
  bool isValid() override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QString notes() override;
};

#endif // ANTIQUACRM_SIZEEDIT_H
