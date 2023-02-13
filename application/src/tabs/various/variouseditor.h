// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_VARIOUSEDITOR_H
#define ANTIQUACRM_VARIOUSEDITOR_H

#include <AntiquaImgEdit>
#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>

class VariousEditor final : public InventoryEditor {
  Q_OBJECT

private:
  // Row:0
  SerialID *va_id;           /**< @brief ReadOnly:ArticleID */
  IntSpinBox *va_count;      /**< @brief Bestandsangabe */
  PriceEdit *va_price;       /**< @brief Preis */
  YearEdit *va_year;         /**< @brief Jahresangabe */
  BoolBox *va_including_vat; /**< @brief inkl. Mehwertsteuer */
  BoolBox *va_restricted;    /**< @brief Öffentlicher verkauf */

  // Row:1
  AntiquaSplitter *m_splitter; /**< @brief Zweite Reihe */
  LineEdit *va_title;          /**< @brief Title */
  LineEdit *va_title_extended; /**< @brief Title erweitert */
  ConditionEdit *va_condition; /**< @brief Umschreibung */
  LineEdit *va_designation;    /**< @brief Zustandsbeschreibung */
  StorageLocation *va_storage; /**< @brief Lagerposition */
  ImageView *m_imageView;
  ImageToolBar *m_imageToolBar;

  // Row:2
  EditorTab *m_tabWidget;             /**< @brief Zusätze */
  TextField *va_description;          /**< @brief Beschreibung */
  TextField *va_internal_description; /**< @brief Interne Beschreibung */
  AntiquaDateInfo *va_since;          /**< @brief Erstellt */
  AntiquaDateInfo *va_changed;        /**< @brief Geändert */

  // Row:3
  EditorActionBar *m_actionBar; /**< @brief Aktionen */

  void setInputFields() override;
  bool setDataField(const QSqlField &field, const QVariant &value) override;
  void importSqlResult() override;
  bool sendSqlQuery(const QString &query) override;
  const QHash<QString, QVariant> createSqlDataset() override;
  void createSqlUpdate() override;
  void createSqlInsert() override;
  bool realyDeactivateEntry();

private Q_SLOTS:
  void setSaveData() override;
  void setCheckLeaveEditor() override;
  void setFinalLeaveEditor(bool force = true) override;
  void actionRemoveImage(qint64 articleId);
  void actionEditImages();

public Q_SLOTS:
  void setRestore() override;

public:
  explicit VariousEditor(QWidget *parent = nullptr);
  bool openEditEntry(qint64 articleId) override;
  bool createNewEntry() override;
};

#endif // ANTIQUACRM_VARIOUSEDITOR_H
