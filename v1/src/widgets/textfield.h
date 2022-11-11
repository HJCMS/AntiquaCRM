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
  void loadDataset();

private Q_SLOTS:
  void dataChanged();

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setValue(const QVariant &);
  void setFocus();
  void setEditable(bool b = true);

public:
  explicit TextField(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_TEXTFIELD_H
