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
 * @ingroup widgets
 */
class DNBSearch final : public QPushButton {
  Q_OBJECT

private:
  QString p_type;
  QMenu *m_menu;
  const QIcon icon() const;
  const QUrl home() const;
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
  void sendQuery(const QUrlQuery &query);

public:
  /**
   * @param parent - parent object
   */
  explicit DNBSearch(QWidget *parent = nullptr);

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
