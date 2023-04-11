// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_BOOKSEDITOR_H
#define ANTIQUACRM_PLUGIN_BOOKSEDITOR_H

#include <AntiquaTabs>
#include <AntiquaWidgets>
#include <QObject>

class ANTIQUACRM_LIBRARY BooksEditor final : public AntiquaCRM::TabsEditor {
  Q_OBJECT

private:
  // Row 0
  AntiquaCRM::SerialId *ib_id;           /**< @brief ReadOnly:ArticleID */
  AntiquaCRM::CrowdEdit *ib_count;       /**< @brief Bestandsangabe */
  AntiquaCRM::PriceEdit *ib_price;       /**< @brief Preis */
  AntiquaCRM::BoolBox *ib_signed;        /**< @brief Signiert? */
  AntiquaCRM::BoolBox *ib_restricted;    /**< @brief Zensiert? */
  // Row 1
  AntiquaCRM::NumEdit *ib_edition;   /**< @brief Ausgabe */
  AntiquaCRM::NumEdit *ib_pagecount; /**< @brief Seitenanzahl */
  AntiquaCRM::NumEdit *ib_weight;    /**< @brief Gewicht */
  AntiquaCRM::YearEdit *ib_year;     /**< @brief Jahr */
  AntiquaCRM::NumEdit *ib_volume;    /**< @brief Band ? */
  // Row 2
  AntiquaCRM::Splitter *m_splitter;        /**< @brief Splitter */
  AntiquaCRM::TextLine *ib_title;          /**< @brief Buch Titel */
  AntiquaCRM::TextLine *ib_title_extended; /**< @brief Ereiterte Titel  */
  AntiquaCRM::TextLine *ib_author;         /**< @brief Buchautor */
  AntiquaCRM::TextLine *ib_publisher;      /**< @brief Herausgeber/Verlag */
  AntiquaCRM::ConditionEdit *ib_condition; /**< @brief Zustands beschreibung */
  AntiquaCRM::SelectLanguage *ib_language; /**< @brief Sprache */
  AntiquaCRM::BookBindingEdit *ib_binding; /**< @brief BuchEinband Kategorie */
  AntiquaCRM::TextLine *ib_designation;    /**< @brief Umschreibung */
  AntiquaCRM::SelectStorage *ib_storage;   /**< @brief Lager bestimmung */
  AntiquaCRM::KeywordsEdit *ib_keyword;    /**< @brief Schlüsselwort */
  AntiquaCRM::IsbnEdit *ib_isbn;           /**< @brief ISBN */

  // Row 3
  AntiquaCRM::TabWidget *m_tabWidget;
  /** @brief Öffentliche Beschreibung */
  AntiquaCRM::TextField *ib_description;
  /** @brief Interne Beschreibung */
  AntiquaCRM::TextField *ib_internal_description;

  /**
   * @brief Beinhaltet Cancel, Restore, Save und go-back.
   * @todo
   */
  AntiquaCRM::TabsEditActionBar *m_actionBar;

  /**
   * @brief Bilder operations Knopfleiste
   * @todo
   */
  AntiquaCRM::ImageToolBar *m_imageToolBar;

  /**
   * @brief Eingebettete Bildansicht
   * @todo
   */
  AntiquaCRM::ImageThumbnail *m_thumbnail;

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
  void setLoadThumbnail(qint64 articleId);
  void setRemoveThumbnail(qint64 articleId);
  void setActionEditImages();

public Q_SLOTS:
  void setRestore() override;

public:
  explicit BooksEditor(QWidget *parent = nullptr);
  bool openEditEntry(qint64 articleId) override;
  bool createNewEntry() override;
};

#endif // ANTIQUACRM_PLUGIN_BOOKSEDITOR_H
