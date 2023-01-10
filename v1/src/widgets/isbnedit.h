// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ISBNEDIT_H
#define ANTIQUACRM_ISBNEDIT_H

#include <AntiquaInputEdit>
#include <QCompleter>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QRegExpValidator>
#include <QRegularExpression>
#include <QString>
#include <QWidget>

/**
 * @brief Eingabe Editor für EAN nach ISO 2108
 * @class IsbnEdit
 * @ingroup Utils
 */
class IsbnEdit final : public InputEdit {
  Q_OBJECT

public:
  enum CodeType {
    ISBNEAN = 0x01, /**< ISBN/EAN BookNumber */
    GTIN12 = 0x02   /**< Universal Product Code (UPC) */
  };
  explicit IsbnEdit(QWidget *parent = nullptr,
                    IsbnEdit::CodeType ctype = IsbnEdit::CodeType::ISBNEAN);

private:
  IsbnEdit::CodeType p_codeType;

  /**
   * @brief Eingabe Editor
   */
  QLineEdit *m_lineEdit;

  /**
   * @brief Einfacher Regulärere ausdruck
   * Dieser wird für QRegExpValidator bei der Einagbe verwendet.
   */
  const QRegExp simplePattern = QRegExp("^([\\d]{10,13})$");
  QRegExpValidator *m_validator;

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

  /**
   * @brief Regulärer Ausdruck für UPC 12, GTIN-12
   * @list Bedeutung der Ersten Zahl:
   * @li 0 - Normaler regulärer UPC Code
   * @li 1 - Reserviert (evtl. für spätere Nutzung)
   * @li 2 - Produkte, die nach Gewicht berechnet werden.
   *     Barcode wird im Geschäft erstellt um Produkt auszuzeichnen
   * @li 3 - National Drug Code (NDC), National Health Related Items Code (HRI).
   * @li 4 - UPC Code, kann ohne Format-Einschränkungen verwendet werden.
   * @li 5 - Coupon
   * @li 6 - Normaler regulärer UPC Code
   * @li 7 - Normaler regulärer UPC Codex
   * @li 8 - Reserviert für spätere Nutzung
   * @li 9 - Reserviert für spätere Nutzung
   * @code
   *  Aufbau nach Zahlenposition {
   *    (0)    Code Prefix
   *    (2-6)  Hersteller des Produkts
   *    (7-11) Artikelnummer
   *    (12)   Prüfziffer
   *  }
   * @endcode
   * @note Die Prüfsumme wird wie bei EAN-13 in Modulo 10 durchgeführt!
   * https://www.activebarcode.de/codes/checkdigit/modulo10
   */
  const QRegularExpression gtin12 = QRegularExpression("^(0[\\d]{11})$");
  bool isGTIN12(const QString &upc) const;

  /**
   * @brief Status Icon Label
   */
  QLabel *m_status;

  void loadDataset();

private Q_SLOTS:
  void setValidationIcon(bool);

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
  void setProperties(const QSqlField &field);
  bool isValid();
  qint64 number();
  const QVariant value();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_ISBNEDIT_H
