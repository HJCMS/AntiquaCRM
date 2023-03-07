// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_DATEINFO_H
#define ANTIQUACRM_DATEINFO_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QDateTimeEdit>

/**
 * @brief The AntiquaDateInfo class
 * @ingroup widgets
 */
class AntiquaDateInfo final : public InputEdit {
  Q_OBJECT

private:
  QDateTimeEdit *m_edit;
  void loadDataset() override;

private Q_SLOTS:
  void dateChanged();

public Q_SLOTS:
  Q_INVOKABLE void reset() override;
  void setValue(const QVariant &) override;
  void setFocus() override;

public:
  explicit AntiquaDateInfo(QWidget *parent = nullptr);
  void setDisplayFormat(const QString &format);
  void setProperties(const QSqlField &field) override;
  const QVariant value() override;
  bool isValid() override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QString notes() override;
};

#endif // ANTIQUACRM_DATEINFO_H
