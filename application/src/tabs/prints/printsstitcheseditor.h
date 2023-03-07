// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PRINTSSTITCHES_EDITOR_H
#define ANTIQUACRM_PRINTSSTITCHES_EDITOR_H

#include <AntiquaImgEdit>
#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>

class KeywordLineEdit;

/**
 * @brief Inventory Prints Editor
 * @ingroup AntiquaPrintsStitches
 */
class PrintsStitchesEditor final : public InventoryEditor {
  Q_OBJECT

private:
  // Row0
  SerialID *ip_id;           /**< @brief ReadOnly:ArticleID */
  IntSpinBox *ip_count;      /**< @brief Bestandsangabe */
  PriceEdit *ip_price;       /**< @brief Preis */
  BoolBox *ip_including_vat; /**< @brief inkl. Mehwertsteuer */
  BoolBox *ip_views;         /**< @brief Ansicht */
  BoolBox *ip_kolorit;       /**< @brief Koloriert */
  BoolBox *ip_landscape;     /**< @brief Format */
  YearEdit *ip_year;         /**< @brief Jahresangabe */
  BoolBox *ip_restricted;    /**< @brief Zensiert? */
  // Row1
  AntiquaSplitter *m_splitter;
  AntiquaILabel *m_infoLabel;  /**< @brief Labels */
  LineEdit *ip_title;          /**< @brief Titel */
  LineEdit *ip_title_extended; /**< @brief Title erweitert */
  LineEdit *ip_author;         /**< @brief Autoren */
  LineEdit *ip_format;         /**< @brief Seitenverhältnis */
  ConditionEdit *ip_condition; /**< @brief Umschreibung */
  LineEdit *ib_designation;    /**< @brief Zustand */
  TechniqueEdit *ip_technique; /**< @brief Darstellungs-technik */
  StorageLocation *ip_storage; /**< @brief Lagerposition */
  KeywordLineEdit *ip_keyword; /**< @brief Shclüsselwörter */
  ImageView *m_imageView;
  ImageToolBar *m_imageToolBar;
  // Row2
  EditorTab *m_tabWidget;
  TextField *ip_description;          /**< @brief Beschreibung */
  TextField *ip_internal_description; /**< @brief Interne Beschreibung */
  AntiquaDateInfo *ip_since;          /**< @brief Erstellt */
  AntiquaDateInfo *ip_changed;        /**< @brief Geändert */
  // Row3
  EditorActionBar *m_actionBar; /**< @brief Actionsbar */

  void setInputFields() override;

  bool setDataField(const QSqlField &field, const QVariant &value) override;

  void importSqlResult() override;

  bool sendSqlQuery(const QString &query) override;

  const QHash<QString, QVariant> createSqlDataset() override;

  void createSqlUpdate() override;

  void createSqlInsert() override;

  bool realyDeactivateEntry();

public Q_SLOTS:
  void setRestore() override;

private Q_SLOTS:
  void setSaveData() override;
  void setCheckLeaveEditor() override;
  void setFinalLeaveEditor(bool force = true) override;
  void actionRemoveImage(qint64 articleId);
  void actionEditImages();

public:
  explicit PrintsStitchesEditor(QWidget *parent = nullptr);
  bool openEditEntry(qint64 articleId) override;
  bool createNewEntry() override;
};

#endif // ANTIQUACRM_PRINTSSTITCHES_EDITOR_H
