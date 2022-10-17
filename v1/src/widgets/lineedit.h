// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_LINEEDIT_H
#define ANTIQUACRM_LINEEDIT_H

#include <AntiquaInputEdit>
#include <QByteArray>
#include <QSqlField>
#include <QObject>
#include <QWidget>

class LineEdit final : public InputEdit {
  Q_OBJECT

private:
  AntiquaLineEdit *m_edit;
  QCompleter *m_completer;
  bool p_passwordInput = false;
  void loadDataset();

private Q_SLOTS:
  void inputChanged(const QString &);

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();
  void setFocus();

Q_SIGNALS:
  void textChanged(const QString &);

public:
  explicit LineEdit(QWidget *parent = nullptr, bool enableStretch = false);
  void setProperties(const QSqlField &field);
  void setPasswordInput(bool);
  bool isPasswordInput();
  void addCompleter(const QStringList &);
  void setInfo(const QString &);
  const QString info();
  const QVariant value();
  bool isValid();
  const QString notes();
};

#endif // ANTIQUACRM_LINEEDIT_H
