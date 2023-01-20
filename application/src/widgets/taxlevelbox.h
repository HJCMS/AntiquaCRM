// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TAXLEVELBOX_H
#define ANTIQUACRM_TAXLEVELBOX_H

#include <AGlobal>
#include <AntiquaInputEdit>

class TaxLevelBox final : public InputEdit {
  Q_OBJECT

private:
  AntiquaComboBox *m_box;
  void loadDataset();

private Q_SLOTS:
  void dataChanged(int);

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setValue(const QVariant &);
  void setFocus();

public:
  explicit TaxLevelBox(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_TAXLEVELBOX_H
