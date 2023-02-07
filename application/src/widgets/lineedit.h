// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_LINEEDIT_H
#define ANTIQUACRM_LINEEDIT_H

#include <AntiquaInputEdit>
#include <QCompleter>

class LineEdit final : public InputEdit {
  Q_OBJECT

private:
  AntiquaLineEdit *m_edit;
  QCompleter *m_completer;
  bool p_passwordInput = false;
  void loadDataset() override;

private Q_SLOTS:
  void inputChanged(const QString &);

public Q_SLOTS:
  Q_INVOKABLE void reset() override;
  void setFocus() override;
  void setValue(const QVariant &) override;

Q_SIGNALS:
  void textChanged(const QString &);

public:
  explicit LineEdit(QWidget *parent = nullptr, bool enableStretch = false);
  void setProperties(const QSqlField &field) override;
  void setPasswordInput(bool);
  bool isPasswordInput();
  void setCompleter(const QStringList &);
  void setInfo(const QString &) override;
  void setReadOnly(bool b = false);
  const QString info() override;
  const QVariant value() override;
  bool isValid() override;
  const QString notes() override;
};

#endif // ANTIQUACRM_LINEEDIT_H
