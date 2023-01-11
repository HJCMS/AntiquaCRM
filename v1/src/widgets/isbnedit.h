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
    ISBNEAN = 0x01, /**< ISBN/EAN Booknumber */
    GTIN13 = 0x02   /**< ENA13/GTIN13 Medianumber */
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
   * @brief EAN13 Prüfsumme berechnen!
   * Der Prüfziffern-Algorithmus des GS1-Systems basiert auf einer Gewichtung
   * der einzelnen Ziffern der zu prüfenden Nummer mit den Faktoren "3" und
   * "1"... von rechts nach links und dem Modulo 10. Mit anderen Worten
   * ausgedrückt: Die einzelnen Ziffern der GTIN-, GLN- oder SSCC-Nummer werden
   * von rechts nach links - also von hinten nach vorne - abwechselnd mit den
   * Faktoren "3" und "1" multipliziert, wobei stets mit Faktor "3" begonnen
   * wird. Die Summe dieser einzelnen Produkte (Produktsumme) ist zu ermitteln.
   * Die Differenz zwischen der Produktsumme und dem nächsten vollen "Zehner"
   * (Aufrundung) ergibt die Prüfziffer. Ergibt sich eine durch 10 teilbare
   * Produktsumme, so ist die Prüfziffer gleich Null.
   */
  bool validateEAN13(const QString ean13) const;

  /**
   * @brief Regulärer Ausdruck für ISBN/EAN 13 (ISO 2108)
   */
  const QRegularExpression p13 = QRegularExpression("^(97[89])([\\d]{10})$");
  bool isISBN13(const QString &isbn) const;

  /**
   * @brief Regulärer Ausdruck für EAN13, GTIN13
   * @code
   *  Aufbau nach Zahlenposition {
   *    (0-9)   Länder und Unternehmenskennung
   *    (10-12) Artikelnummer
   *    (13)    Prüfziffer
   *  }
   * @endcode
   * https://www.gs1-germany.de/serviceverzeichnis/pruefziffernrechner/
   */
  const QRegularExpression gtin13 = QRegularExpression("^([\\d]{13})$");
  bool isGTIN13(const QString &ean) const;

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
