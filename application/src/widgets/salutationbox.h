// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SALUTATIONBOX_H
#define ANTIQUACRM_SALUTATIONBOX_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QLineEdit>
#include <QMap>
#include <QObject>
#include <QWidget>

class SalutationBox final : public InputEdit {
  Q_OBJECT

private:
  AntiquaComboBox *m_box;
  QLineEdit *m_edit;
  void loadDataset() override;
  inline const QString withoutDisclosures();

private Q_SLOTS:
  void dataChanged(int);

public Q_SLOTS:
  Q_INVOKABLE void reset() override;
  void setValue(const QVariant &) override;
  void setFocus() override;

public:
  explicit SalutationBox(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field) override;
  const QVariant value() override;
  bool isValid() override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QString notes() override;
  static const QMap<QString,QString> salutations();
};

#endif // ANTIQUACRM_SALUTATIONBOX_H
