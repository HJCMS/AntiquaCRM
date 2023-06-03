// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_DESCRIPEEDIT_H
#define ANTIQUACRM_DESCRIPEEDIT_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QTextEdit>
#include <QListWidget>
#include <QListWidgetItem>

class DescripeEdit final : public InputEdit {
  Q_OBJECT

private:
  QTextEdit *m_edit;
  QListWidget *m_list;
  void loadDataset();

private Q_SLOTS:
  void addTextModule(QListWidgetItem *);
  void dataChanged();

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setValue(const QVariant &);
  void setFocus();

public:
  explicit DescripeEdit(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_DESCRIPEEDIT_H
