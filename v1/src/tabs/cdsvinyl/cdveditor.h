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

class CDVEditor final : public InventoryEditor {
  Q_OBJECT

private:
  // Row 0
  SerialID *cv_id;           /**< @brief ReadOnly:ArticleID */
  IntSpinBox *cv_count;      /**< @brief Bestandsangabe */
  PriceEdit *cv_price;       /**< @brief Preis */
  YearEdit *cv_year;         /**< @brief Jahresangabe */
  BoolBox *cv_including_vat; /**< @brief inkl. Mehwertsteuer */
  BoolBox *cv_restricted;    /**< @brief Öffentlicher verkauf */

  // Row 1
  QSplitter *m_splitter;       /**< @brief Zweite Reihe */
  AntiquaILabel *m_infoLabel;  /**< @brief Labels */
  LineEdit *cv_title;          /**< @brief Titel */
  LineEdit *cv_title_extended; /**< @brief Title erweitert */
  LineEdit *cv_author;         /**< @brief Gruppe oder Band */
  SelectMediaType *cv_type;    /**< @brief Mediatype */
  ConditionEdit *cv_condition; /**< @brief Umschreibung */
  StorageLocation *cv_storage; /**< @brief Lagerposition */
  KeywordLineEdit *cv_keyword; /**< @brief Schlüsselwörter */
  IsbnEdit *cv_isbn;           /**< @brief ISBN/EAN */
  ImageView *m_imageView;
  ImageToolBar *m_imageToolBar;

  // Row 2
  EditorTab *m_tabWidget;             /**< @brief Zusätze */
  TextField *cv_description;          /**< @brief Beschreibung */
  TextField *cv_internal_description; /**< @brief Interne Beschreibung */
  AntiquaDateInfo *cv_since;          /**< @brief Erstellt */
  AntiquaDateInfo *cv_changed;        /**< @brief Geändert */

  // Row 3
  EditorActionBar *m_actionBar; /**< @brief Aktionen */

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
  void setFinalLeaveEditor() override;
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
