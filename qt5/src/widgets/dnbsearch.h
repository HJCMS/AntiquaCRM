// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUA_WIDGET_DNBSEARCH_H
#define ANTIQUA_WIDGET_DNBSEARCH_H

#include <QAction>
#include <QMenu>
#include <QObject>
#include <QPushButton>
#include <QUrl>
#include <QUrlQuery>
#include <QWidget>

/**
 * @brief The DNBSearch Button class
 *
 * This Button Class opens the search page of the German National Library in
 * your web browser. It creates a query urn to the katalog.dnb.de Library with
 * call QDesktopServices::openUrl.
 *
 * @note It does not prepare the search query, this must done by subclasses.
 *
 * Documentations and Urls:
 * @li https://katalog.dnb.de/
 * @li https://blog.dnb.de/dnbkatalogbeta/
 *
 * @ingroup widgets
 */
class DNBSearch final : public QPushButton {
  Q_OBJECT

private:
  QString p_type;
  QMenu *m_menu;
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
  explicit DNBSearch(QWidget *parent = nullptr);

  enum Option {
    ANY = 0x1,     /**< @brief Search in everything */
    TITLE = 0x2,   /**< @brief Title search */
    AUTHOR = 0x4,  /**< @brief Authors search */
    TITAUT = 0x8,  /**< @brief Title and Author search */
    NUMERIC = 0x10 /**< @brief ISBN search */
  };

  /**
   * @brief Current Button Options
   * @code
   *   QMap<DNBSearch::Option, QString>
   *   QMap<ENUM, Description>
   * @endcode
   */
  const QMap<DNBSearch::Option, QString> options() const;

  /**
   * @brief Responses the current search type.
   */
  const QString getSearchType();

  /**
   * @brief List of all acceptable Search parameter types.
   */
  static const QStringList params();
};

#endif // ANTIQUA_WIDGET_DNBSEARCH_H
