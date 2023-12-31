// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_CATALOG_SEARCHBUTTON_H
#define ANTIQUACRM_PLUGIN_CATALOG_SEARCHBUTTON_H

#include <AntiquaWidgets>
#include <QAction>
#include <QIcon>
#include <QMap>
#include <QMenu>
#include <QObject>
#include <QPushButton>
#include <QStringList>
#include <QUrlQuery>

/**
 * @class CatalogSearchButton
 * @brief DNB Catalog search button with menu class
 *
 * This Button Class opens the search page of the German National Library in
 * your web browser. It creates a query urn to the katalog.dnb.de Library to
 * call a get request with QDesktopServices::openUrl.
 *
 * @note It does not prepare the search query, this must done by subclasses.
 *
 * Documentations:
 * @li https://katalog.dnb.de/
 * @li https://blog.dnb.de/dnbkatalogbeta/
 */
class ANTIQUACRM_LIBRARY CatalogSearchButton final : public QPushButton {
  Q_OBJECT

private:
  QString p_type;
  QMenu *m_menu;

  /**
   * @brief Default search icon
   */
  const QIcon icon() const;

  /**
   * @brief Default catalog Homepage
   * @li https://katalog.dnb.de/DE/home.html
   */
  const QUrl home() const;

  /**
   * @brief Query catalog in list mode
   * @li https://katalog.dnb.de/DE/list.html
   */
  const QUrl list() const;

private Q_SLOTS:
  void prepareAction(QAction *);

Q_SIGNALS:
  /**
   * @brief Send query if button has clicked.
   *
   * This Signal is emitted when prepareAction has finished successfully.
   */
  void sendQuery();

public Q_SLOTS:
  /**
   * @brief Creates Search URL with given Query statement
   *
   * Opens a link with Default Web-Browser.
   *
   * @param query - prepared query
   */
  void openLink(const QUrlQuery &query);

public:
  /**
   * @param parent - parent object
   */
  explicit CatalogSearchButton(QWidget *parent = nullptr);

  /**
   * @brief Available button options.
   */
  enum Option {
    ANY = 0x1,     /**< @brief Search in everything */
    TITLE = 0x2,   /**< @brief Title search */
    AUTHOR = 0x4,  /**< @brief Authors search */
    TITAUT = 0x8,  /**< @brief Title and Author search */
    NUMERIC = 0x10 /**< @brief ISBN search */
  };

  /**
   * @brief Map with configured button options
   * @code
   *   QMap<CatalogSearchButton::Option, QString>
   *   QMap<ENUM, „Translated title“>
   * @endcode
   */
  const QMap<CatalogSearchButton::Option, QString> options() const;

  /**
   * @brief Responses the current search type.
   */
  const QString getSearchType();

  /**
   * @brief List of all acceptable Search parameter types.
   */
  static const QStringList params();
};

#endif // ANTIQUACRM_PLUGIN_CATALOG_SEARCHBUTTON_H
