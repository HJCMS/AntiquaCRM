// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TEXTFIELD_H
#define ANTIQUACRM_TEXTFIELD_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QTextEdit>

class TextField final : public InputEdit {
  Q_OBJECT

private:
  QTextEdit *m_edit;
  void loadDataset() override;

private Q_SLOTS:
  void dataChanged();

public Q_SLOTS:
  Q_INVOKABLE void reset() override;
  void setValue(const QVariant &) override;
  void setFocus() override;
  void setEditable(bool b = true);

public:
  explicit TextField(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field) override;
  const QVariant value() override;
  bool isValid() override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QString notes() override;
};

#endif // ANTIQUACRM_TEXTFIELD_H
