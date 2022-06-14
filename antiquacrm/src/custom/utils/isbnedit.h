// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ISBNEDIT_UTILS_H
#define ISBNEDIT_UTILS_H

#include <QCompleter>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRegExpValidator>
#include <QRegularExpression>
#include <QString>
#include <QWidget>
#include <UtilsMain>

/**
 * @brief Eingabe Editor für ISBN/EAN nach ISO 2108
 * @class IsbnEdit
 * @ingroup Utils
 * https://de.wikipedia.org/wiki/Internationale_Standardbuchnummer
 * https://german-isbn.de/isbn/isbn-konverter
 * Eine ISBN ist vom Type "signed long long" __int64
 */
class IsbnEdit final : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
   * @brief Info Label
   */
  QLabel *m_info;

  /**
   * @brief Eingabe Editor
   */
  QLineEdit *m_isbn;

  /**
   * @brief Knopf für ISBN Onlineabfrage
   */
  QPushButton *m_query;

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
  const bool isISBN10(const QString &isbn) const;

  /**
   * @brief Regulärer Ausdruck für ISBN/EAN 13 (ISO 2108)
   */
  const QRegularExpression p13 = QRegularExpression("^(97[89])([\\d]{10})$");
  const bool isISBN13(const QString &isbn) const;

  /**
   * @brief Berechnen Prüfen
   * @param isbn Nummer
   * @return Validierung
   * @note Nur @b ISBN-13 wird hier berechnet!
   */
  bool calculate(const QString &isbn) const;

  /**
   * @brief Visueller Hinweis für den Anwender erstellen!
   */
  void setButtonText(int);

private Q_SLOTS:
  void feedbackIcon(bool);

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
  bool isValid();
  qint64 number();
  const QVariant value();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ISBNEDIT_UTILS_H
