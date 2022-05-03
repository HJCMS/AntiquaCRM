// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef LINEEDIT_UTILS_H
#define LINEEDIT_UTILS_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtWidgets/QCompleter>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>
#include <UtilsMain>

class LineEdit : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QLabel *m_label;
  QLineEdit *m_edit;
  QCompleter *m_completer;
  bool p_passwordInput = false;

private Q_SLOTS:
  void inputChanged(const QString &);

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();
  void setFocus();

public:
  explicit LineEdit(QWidget *parent = nullptr);

  /**
   * @brief setPasswordInput
   * @note Wenn gesetzt gibt @ref value() ein ByteArray zurück!
   */
  void setPasswordInput(bool);

  /**
   * @brief Erscheinungsbild einschränken
   * @param length Zeichenlänge einschränken
   * @param width  Weite einschränken
   */
  void restrictDisplay(int length, int width);

  /**
   *  @note Die Liste darf nicht Leer sein!
   */
  void addCompleter(const QStringList &);

  void setInfo(const QString &);
  const QString info();
  void setPlaceholderText(const QString &);
  const QVariant value();
  bool isValid();
  const QString notes();
};

#endif // LINEEDIT_UTILS_H
