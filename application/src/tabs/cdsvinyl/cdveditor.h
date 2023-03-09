// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_CDVEDITOR_H
#define ANTIQUACRM_CDVEDITOR_H

#include <AntiquaImgEdit>
#include <AntiquaWidgets>
#include <QObject>
#include <QSplitter>
#include <QWidget>

class KeywordLineEdit;

/**
 * @brief CD+Vinyl Editor
 * @ingroup _cdvinyl
 */
class CDVEditor final : public InventoryEditor {
  Q_OBJECT

private:
  // Row:0
  SerialID *cv_id;           /**< @brief ReadOnly:ArticleID */
  IntSpinBox *cv_count;      /**< @brief Bestandsangabe */
  PriceEdit *cv_price;       /**< @brief Preis */
  YearEdit *cv_year;         /**< @brief Jahresangabe */
  BoolBox *cv_including_vat; /**< @brief inkl. Mehwertsteuer */
  BoolBox *cv_restricted;    /**< @brief Öffentlicher verkauf */

  // Row:1
  AntiquaSplitter *m_splitter;  /**< @brief Zweite Reihe */
  LineEdit *cv_title;           /**< @brief Title */
  LineEdit *cv_title_extended;  /**< @brief Title erweitert */
  LineEdit *cv_author;          /**< @brief Artist */
  LineEdit *cv_publisher;       /**< @brief Herausgeber */
  SelectMediaType *cv_mtype;    /**< @brief Mediatype */
  ConditionEdit *cv_condition;  /**< @brief Umschreibung */
  LineEdit *cv_designation;     /**< @brief Zustandsbeschreibung */
  StorageLocation *cv_storage;  /**< @brief Lagerposition */
  KeywordLineEdit *cv_keyword;  /**< @brief Schlüsselwörter */
  IsbnEdit *cv_eangtin;         /**< @brief EAN13/GTIN13 */
  QPushButton *btn_cdread;      /**< @brief Dialog ReadCD ID's */
  ImageToolBar *m_imageToolBar; /**< @brief Bilder Editieren */
  ImageView *m_imageView;       /**< @brief Bildvorschau */

  // Row:2
  EditorTab *m_tabWidget;             /**< @brief Zusätze */
  TextField *cv_description;          /**< @brief Beschreibung */
  TextField *cv_internal_description; /**< @brief Interne Beschreibung */
  AntiquaDateInfo *cv_since;          /**< @brief Erstellt */
  AntiquaDateInfo *cv_changed;        /**< @brief Geändert */

  // Row:3
  EditorActionBar *m_actionBar; /**< @brief Aktionen */

  void setCDsDefault();
  void setInputFields() override;
  bool setDataField(const QSqlField &field, const QVariant &value) override;
  void importSqlResult() override;
  bool sendSqlQuery(const QString &query) override;
  const QHash<QString, QVariant> createSqlDataset() override;
  void createSqlUpdate() override;
  void createSqlInsert() override;
  bool realyDeactivateEntry();
  bool checkYear();

private Q_SLOTS:
  void setSaveData() override;
  void setCheckLeaveEditor() override;
  void setFinalLeaveEditor(bool force = true) override;
  void openReadCDDialog();
  void actionRemoveImage(qint64 articleId);
  void actionEditImages();

public Q_SLOTS:
  void setRestore() override;

public:
  explicit CDVEditor(QWidget *parent = nullptr);
  bool openEditEntry(qint64 articleId) override;
  bool createNewEntry() override;
};

#endif // ANTIQUACRM_CDVEDITOR_H
