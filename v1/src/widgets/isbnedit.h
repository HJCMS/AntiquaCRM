// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ISBNEDIT_H
#define ANTIQUACRM_ISBNEDIT_H

#include <QCompleter>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QRegExpValidator>
#include <QRegularExpression>
#include <QString>
#include <QWidget>
#include <AntiquaInputEdit>

/**
 * @brief Eingabe Editor für ISBN/EAN nach ISO 2108
 * @class IsbnEdit
 * @ingroup Utils
 * https://de.wikipedia.org/wiki/Internationale_Standardbuchnummer
 * https://german-isbn.de/isbn/isbn-konverter
 * Eine ISBN ist vom Type "signed long long" __int64
 */
class IsbnEdit final : public InputEdit {
  Q_OBJECT

private:
  /**
   * @brief Eingabe Editor
   */
  QLineEdit *m_isbn;

  /**
   * @brief Einfacher Regulärere ausdruck
   * Dieser wird für QRegExpValidator bei der Einagbe verwendet.
   */
  const QRegExp simplePattern = QRegExp("^([\\d]{10,13})$");
  QRegExpValidator *m_validator;

  /**
   * @brief ISBN-13 Buchpräfixe für QCompleter
   */
  const QStringList prefix = QStringList({"978", "979"});
  QCompleter *m_completer;

  /**
   * @brief Regulärer Ausdruck für ISBN 10 (ALT)
   */
  const QRegularExpression p10 = QRegularExpression("^(?!97[89])([\\d]{10})$");
  bool isISBN10(const QString &isbn) const;

  /**
   * @brief Regulärer Ausdruck für ISBN/EAN 13 (ISO 2108)
   */
  const QRegularExpression p13 = QRegularExpression("^(97[89])([\\d]{10})$");
  bool isISBN13(const QString &isbn) const;

  void loadDataset();

private Q_SLOTS:
  /**
   * @brief Ableitung von QLineEdit::textChanged
   */
  void isbnChanged(const QString &);

Q_SIGNALS:
  /**
   * @brief Wird während der Eingabe ausgelöst.
   */
  void isbnIsValid(bool);

  /**
   * @brief Der Abfrage Knopf wurde ausgelöst.
   */
  void clicked();

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();
  void setFocus();

public:
  explicit IsbnEdit(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field);
  bool isValid();
  qint64 number();
  const QVariant value();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_ISBNEDIT_H
