// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TECHNIQUEEDIT_H
#define ANTIQUACRM_TECHNIQUEEDIT_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QCompleter>

class TechniqueEdit final : public InputEdit {
  Q_OBJECT

private:
  AntiquaComboBox *m_comboBox;
  AntiquaLineEdit *m_search;
  QCompleter *m_completer;

private Q_SLOTS:
  void setDataChanged(int);
  void setCompleted();

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setValue(const QVariant &);
  void setFocus();

public:
  explicit TechniqueEdit(QWidget *parent = nullptr);
  void loadDataset();
  void setProperties(const QSqlField &field);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_TECHNIQUEEDIT_H
