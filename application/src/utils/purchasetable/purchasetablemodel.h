// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PURCHASE_TABLEMODEL_H
#define ANTIQUACRM_PURCHASE_TABLEMODEL_H

#include <AntiquaCRM>
#include <QAbstractTableModel>
#include <QList>
#include <QMap>
#include <QObject>
#include <QWidget>

/**
 * @brief Model Klasse für Artikel Einkaufstabelle
 * @section widgets
 */
class PurchaseTableModel final : public QAbstractTableModel {
  Q_OBJECT

private:
  /**
   * @brief Anzeige der Währung bei Preisangaben.
   * Wird aus der Konfiguration beim Initialisieren eingelesen.
   */
  QString currency;

  /**
   * @brief Zellenanzahl
   */
  int table_columns = 11;

  /**
   * @brief Soll verhindern das die SQL Feldgrenzen überschritten werden.
   */
  int max_string_length = 79;

  /**
   * @brief Tabelleninhalt
   */
  QMap<int, AntiquaCRM::OrderArticleItems> articleRows;

public:
  explicit PurchaseTableModel(QObject *parent = nullptr);

  /**
   * @brief Gibt den Typ des aktuellen Artikels als Text zurück.
   * @see AntiquaCRM::ArticleType
   */
  static const QString articleType(int type);

  /**
   * @brief Beinhaltet die SQL-Tabellen Feldnamen und Kopzeilenübersetzungen!
   * @note Die Feldnamen müssen in der Klasse AProviderOrder definiert sein!
   * Andernfalls kann es dazu kommen das der falsche Datentyp ausgegeben wird!
   */
  static const AntiquaCRM::ATableHeaderColumn headerColumn(int column);

  /**
   * @brief Gibt die Liste der Editierbaren Zellen zurück!
   */
  static const QList<int> editableColumns();

  /**
   * @brief Eine neue Artikelbestellung hinzufügen.
   */
  bool addArticle(const AntiquaCRM::OrderArticleItems &item);

  /**
   * @brief Denn kompletten Tabelleninhalt ohne die Kopfdaten löschen!
   */
  void clear();

  /**
   * @brief Einen Eintrag aus @ref articleRows entfernen!
   * @warning !!! KEIN STANDARD WIE IN DER DOKUMENTATION VORGEGEBEN !!!
   * @bug Wenn ich hier, wie in der Dokumentation beschrieben mit \i
   * beginRemoveRows und \i endRemoveRows arbeite! Werden Tabelleninhalte bei
   * einem QMap \b nicht richtig verarbeitet.
   * Es wird z.B. bei einem darauf folgenden \i rowCount() der alter Wert zurück
   * gegeben obwohl sich die Größe von \b articleRows geändert hat.
   * Weil die Anzahl der Bestellungen kaum die 10 Übersteigt ist es einfacher
   * die Tabelle zu leeren und alle Daten neu ein zu lesen, das Funkioniert.
   */
  bool removeRows(int row, int count,
                  const QModelIndex &parent = QModelIndex()) override;

  /**
   * @brief Komplette Artikelbestellungs liste einfügen.
   */
  bool addArticles(const QList<AntiquaCRM::OrderArticleItems> &items);

  /**
   * @brief Gibt die Größe von @ref articleRows zurück.
   */
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  /**
   * @brief Die Anzahl der Zellen ist immer gleich!
   * @see p_columns
   */
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  /**
   * @brief Tabellenkopfzeilen zurückgeben.
   * @note Bei den vertikalen Kopfdaten wird die Anzeige der Zeilenzahl auf den
   * Menschlich logischen Wert korrigiert!
   */
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;

  /**
   * @brief Schreibe Daten in Zellen ...
   */
  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole) override;

  /**
   * @brief Gibt den SQL-Tabellen Feldnamen anhand des Zelleindex zurück.
   */
  const QString field(const QModelIndex &index) const;

  /**
   * @brief Gibt den Zellenindex anhand des SQL-Tabellen Feldnamen zurück.
   */
  int columnIndex(const QString &fieldName) const;

  /**
   * @brief Zellendaten einer Tabellenreihe
   */
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

  /**
   * @brief Bearbeitungs Kennzeichen
   * @default (Qt::ItemIsSelectable | Qt::ItemIsEnabled)
   * @list Editierbare Zellen @ref editableColumns() besitzen:
   *  @li Qt::ItemIsSelectable
   *  @li Qt::ItemIsEnabled
   *  @li Qt::ItemIsEditable
   */
  Qt::ItemFlags flags(const QModelIndex &index) const override;
};

#endif // ANTIQUACRM_PURCHASE_TABLEMODEL_H
