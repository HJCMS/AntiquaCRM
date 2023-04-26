// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_ISBNEDIT_H
#define ANTIQUACRM_WIDGETS_ISBNEDIT_H

#include <AntiquaInput>
#include <QObject>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class IsbnEdit
 * @brief Simple ISBN Editor based on EAN/ISO 2108
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY IsbnEdit final : public AntiquaCRM::AbstractInput {
  Q_OBJECT

public:
  enum CodeType {
    ISBN = 0x01, /**< @brief ISBN/EAN Booknumber */
    GTIN = 0x02  /**< @brief UPC-A, GTIN-12, GTIN13, UPC-E Medianumber */
  };

private:
  IsbnEdit::CodeType p_codeType;
  ALineEdit *m_edit;

  /**
   * @brief Einfacher Regulärer ausdruck
   * Dieser wird für QRegExpValidator bei der Einagbe verwendet.
   */
  QRegularExpressionValidator *m_validator;

  /**
   * @brief Regulärer Ausdruck für ISBN 10 (ALT)
   */
  const QRegularExpression p10 = QRegularExpression("^(?!97[89])([\\d]{10})$");
  bool isISBN10(const QString &isbn) const;

  /**
   * @brief UPC/GTIN Prüfsumme berechnen!
   * Der Prüfziffern-Algorithmus des GS1-Systems basiert auf einer Gewichtung
   * der einzelnen Ziffern der zu prüfenden Nummer mit den Faktoren "3" und
   * "1"... von rechts nach links und dem Modulo 10. Mit anderen Worten
   * ausgedrückt: Die einzelnen Ziffern der GTIN-, GLN- oder SSCC-Nummer werden
   * von rechts nach links - also von hinten nach vorne - abwechselnd mit den
   * Faktoren "3" und "1" multipliziert, wobei stets mit Faktor "3" begonnen
   * wird. Die Summe dieser einzelnen Produkte (Produktsumme) ist zu ermitteln.
   * Die Differenz zwischen der Produktsumme und dem nächsten vollen "Zehner"
   * (Aufrundung) ergibt die Prüfziffer. Ergibt sich eine durch 10 teilbare
   * Produktsumme, so ist die Prüfziffer gleich 0.
   */
  bool validateGTIN(const QString &ean, int type = 13) const;

  /**
   * @brief Regulärer Ausdruck für ISBN/EAN 13 (ISO 2108)
   */
  const QRegularExpression p13 = QRegularExpression("^(97[89])([\\d]{10})$");
  bool isISBN13(const QString &isbn) const;

  /**
   * @brief Regulärer Ausdruck UPC-A, GTIN-12, GTIN13 und UPC-E
   * https://www.gs1-germany.de/serviceverzeichnis/pruefziffernrechner/
   */
  const QRegularExpression gtin = QRegularExpression("^([\\d]{12,13})$");
  bool isGTIN(const QString &ean) const;

  void initData() override;

private Q_SLOTS:
  void valueChanged(const QString &);

public Q_SLOTS:
  void setValue(const QVariant &) override;

  void setFocus() override;

  void reset() override;

public:
  /**
   * @param parent - parent widget
   */
  explicit IsbnEdit(QWidget *parent = nullptr,
                    IsbnEdit::CodeType ctype = IsbnEdit::CodeType::ISBN);

  void setRestrictions(const QSqlField &) override;

  void setInputToolTip(const QString &) override;

  void setBuddyLabel(const QString &) override;

  bool isValid() override;

  qint64 number();

  const QMetaType getType() const override;

  const QVariant getValue() override;

  const QString popUpHints() override;

  const QString statusHints() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_ISBNEDIT_H
