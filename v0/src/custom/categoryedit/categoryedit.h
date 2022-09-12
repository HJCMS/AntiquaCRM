// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CATEGORYEDIT_H
#define CATEGORYEDIT_H

#include <QDialog>
#include <QListWidget>
#include <QObject>
#include <QPushButton>
#include <QSplitter>
#include <QStatusBar>
#include <QWidget>

#include <SqlCore>
#include "categorymapping.h"

class CategoryActions;
class CategoryTree;
class DragListWidget;

/**
 * @brief Kategorien bearbeiten Dialog
 * @group CategoryEdit
 */
class CategoryEdit : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
   * @brief HJCMS SqlCore
   */
  HJCMS::SqlCore *m_sql;

  /**
   * @brief Splitter
   */
  QSplitter *m_splitter;

  /**
   * @brief Kategorien Baumansicht
   */
  CategoryTree *m_tree;

  /**
   * @brief Kategorien aus PgSQL::{ref_storage_location.sl_identifier}
   */
  DragListWidget *m_storageList;

  /**
   * @brief Schlüsselwörter aus PgSQL::{inventory_books.ib_keyword}
   */
  DragListWidget *m_keywordsList;

  CategoryActions *m_btnFrame;

  QStatusBar *m_statusBar;

  /**
   * @brief Nachrichtenanzeige Ablaufzeit
   */
  const int timeout = 6000;

  /**
   * @brief Lese Kategorien aus Datenbank und füge sie in CategoryTree ein.
   */
  bool initCategories();

  /**
   * @brief Lese Schlüsselwörter aus Tabellen Lager und Bücher.
   */
  bool initKeywords();

private Q_SLOTS:
  /**
   * @brief Aktuelle Baumansicht in Datenbank speichern!
   */
  void saveCompanyTreeUsage();

private Q_SLOTS:
  /**
   * @brief SQL Datensatz Kategorie Sichtbarkeit ändern!
   */
  void updateCompanyUsage(int categoryId, bool usage);

  /**
   * @brief SQL Datensatz Kategorien Sichtbarkeit ändern!
   * @param ids - PgSQL::{categories_extern.ce_id} Liste
   */
  void disableCompanyUsageList(const QStringList &ids);

protected:
  void keyPressEvent(QKeyEvent *);

public:
  explicit CategoryEdit(QWidget *parent = nullptr);

  /**
   * @brief Tabellen werden erst beim start eingelesen!
   */
  int exec() override;
};

#endif // CATEGORYEDIT_H
