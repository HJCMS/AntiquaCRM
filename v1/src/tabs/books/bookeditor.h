// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_BOOKEDITOR_H
#define ANTIQUACRM_BOOKEDITOR_H

#include <AntiquaInputEdit>
#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>

class KeywordLineEdit;

class BookEditor final : public InventoryEditor {
  Q_OBJECT

private:
  // Row 0
  SerialID *ib_id;           /**< @brief ReadOnly:ArticleID */
  IntSpinBox *ib_count;      /**< @brief Bestandsangabe */
  PriceEdit *ib_price;       /**< @brief Preis */
  BoolBox *ib_including_vat; /**< @brief inkl. Mehwertsteuer */
  BoolBox *ib_signed;        /**< @brief Signiert? */
  BoolBox *ib_restricted;    /**< @brief Zensiert? */
  // Row 1
  IntSpinBox *ib_edition;   /**< @brief Ausgabe */
  IntSpinBox *ib_pagecount; /**< @brief Seitenanzahl */
  IntSpinBox *ib_weight;    /**< @brief Gewicht */
  YearEdit *ib_year;        /**< @brief Jahr */
  IntSpinBox *ib_volume;    /**< @brief Band ? */
  // Row 2
  LineEdit *ib_title;          /**< @brief Buch Titel */
  LineEdit *ib_title_extended; /**< @brief Ereiterte Titel  */
  LineEdit *ib_author;         /**< @brief Buchautor */
  LineEdit *ib_publisher;      /**< @brief Herausgeber/Verlag */
  ConditionEdit *ib_condition; /**< @brief Zustands beschreibung */
  Language *ib_language;       /**< @brief Sprache */
  BookBinding *ib_binding;     /**< @brief BuchEinband Kategorie */
  LineEdit *ib_designation;    /**< @brief Umschreibung */
  StorageLocation *ib_storage; /**< @brief Lager bestimmung */
  KeywordLineEdit *ib_keyword; /**< @brief Schlüsselwort */
  IsbnEdit *ib_isbn;           /**< @brief ISBN */

  // Row 3
  EditorTab *m_tabWidget;
  TextField *ib_description;          /**< @brief Öffentliche Beschreibung */
  TextField *ib_internal_description; /**< @brief Interne Beschreibung */
  AntiquaDateInfo *ib_since;          /**< @brief Ersteller-Datum */
  AntiquaDateInfo *ib_changed;        /**< @brief Änderungs-Datum */

  const QRegularExpression fieldPattern;
  void setInputList();
  bool setDataField(const QSqlField &field, const QVariant &value);
  void importSqlResult();
  bool sendSqlQuery(const QString &query);
  const QHash<QString, QVariant> createSqlDataset();
  void createSqlUpdate();
  void createSqlInsert();

private Q_SLOTS:
  void setSaveData();
  void setCheckLeaveEditor();
  void setFinalLeaveEditor();

public Q_SLOTS:
  void setRestore();

public:
  explicit BookEditor(QWidget *parent = nullptr);
  bool openEditEntry(qint64 articleId);
  bool createNewEntry();
};

#endif // ANTIQUACRM_BOOKEDITOR_H
