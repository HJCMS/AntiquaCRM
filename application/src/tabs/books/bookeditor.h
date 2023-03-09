// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_BOOKEDITOR_H
#define ANTIQUACRM_BOOKEDITOR_H

#include <AntiquaImgEdit>
#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>

class KeywordLineEdit;

/**
 * @brief Inventory Books Editor
 * @ingroup _books
 */
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
  AntiquaSplitter *m_splitter; /**< @brief Splitter */
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

  /**
   * @brief Beinhaltet Cancel, Restore, Save und go-back.
   */
  EditorActionBar *m_actionBar;

  /**
   * @brief Bilder operations Knopfleiste
   */
  ImageToolBar *m_imageToolBar;

  /**
   * @brief Eingebettete Bildansicht
   */
  ImageView *m_imageView;

  void setInputFields() override;

  bool setDataField(const QSqlField &field, const QVariant &value) override;

  void importSqlResult() override;

  bool sendSqlQuery(const QString &query) override;

  const QHash<QString, QVariant> createSqlDataset() override;

  void createSqlUpdate() override;

  void createSqlInsert() override;

  /**
   * Wenn der Benutzer den Artikel Bestand auf 0 setzt!
   * Einen Hinweis ausgeben das der Artikel gleichzeitig
   * auch aus dem Auftrags-System geworfen wird und die
   * Online Shops auf einen Löschenauftrag erhalten!
   * @note Die Meldung wird in @ref saveData ausgelöst!
   */
  bool realyDeactivateEntry();

private Q_SLOTS:
  void setSaveData() override;
  void setCheckLeaveEditor() override;
  void setFinalLeaveEditor(bool force = true) override;
  void setPrintBookCard();
  void actionRemoveImage(qint64 articleId);
  void actionEditImages();

public Q_SLOTS:
  void setRestore() override;

public:
  explicit BookEditor(QWidget *parent = nullptr);
  bool openEditEntry(qint64 articleId) override;
  bool createNewEntry() override;
};

#endif // ANTIQUACRM_BOOKEDITOR_H
