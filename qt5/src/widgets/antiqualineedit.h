// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUA_INPUT_LINEEDIT_H
#define ANTIQUA_INPUT_LINEEDIT_H

#include <QCompleter>
#include <QFocusEvent>
#include <QLineEdit>
#include <QObject>
#include <QValidator>
#include <QWidget>

/**
 * @brief The AntiquaInputValidator class
 *
 * SQL injection protection
 *
 * @ingroup widgets
 */
class AntiquaInputValidator final : public QValidator {
  Q_OBJECT

private:
  static const QStringList checkList();

public:
  explicit AntiquaInputValidator(QLineEdit *parent);
  QValidator::State validate(QString &input, int &pos) const override;
};

/**
 * @brief The AntiquaLineEdit class
 * @ingroup widgets
 */
class AntiquaLineEdit final : public QLineEdit {
  Q_OBJECT
  Q_PROPERTY(bool keyReturn READ keyEnterEventEnabled WRITE
                 setEnableKeyEnterEvent NOTIFY sendKeyEnterEventChanged);

private:
  bool keyEnterEvent;
  AntiquaInputValidator *m_validator;

private Q_SLOTS:
  void checkInput(const QString &);

protected:
  void focusInEvent(QFocusEvent *event);

protected Q_SLOTS:
  virtual void checkEnterEventPressed();

Q_SIGNALS:
  void sendKeyEnterEventChanged();

public:
  explicit AntiquaLineEdit(QWidget *parent);
  void setEnableKeyEnterEvent(bool);
  bool keyEnterEventEnabled();
};

#endif // ANTIQUA_INPUT_LINEEDIT_H
