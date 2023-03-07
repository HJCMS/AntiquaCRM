// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TECHNIQUEEDIT_H
#define ANTIQUACRM_TECHNIQUEEDIT_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QCompleter>

/**
 * @brief The TechniqueEdit class
 * @ingroup widgets
 */
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
  Q_INVOKABLE void reset() override;
  void setValue(const QVariant &) override;
  void setFocus() override;

public:
  explicit TechniqueEdit(QWidget *parent = nullptr);
  void loadDataset() override;
  void setProperties(const QSqlField &field) override;
  const QVariant value() override;
  bool isValid() override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QString notes() override;
};

#endif // ANTIQUACRM_TECHNIQUEEDIT_H
