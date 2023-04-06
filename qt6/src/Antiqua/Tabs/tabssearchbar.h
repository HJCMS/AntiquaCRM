// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_TABS_SEARCHBAR_H
#define ANTIQUACRM_TABS_SEARCHBAR_H

#include <AGlobal>
#include <QObject>
#include <QRegularExpression>
#include <QtWidgets>

namespace AntiquaCRM {

class ANTIQUACRM_LIBRARY TabsSearchBar : public QToolBar {
  Q_OBJECT
  Q_PROPERTY(int minLength READ getMinLength WRITE setMinLength NOTIFY
                 sendMinLengthChanged)

public:
  /**
   * @brief settings confiness for search syntax
   * Enumeration to restrict search to:
   *  @li ANYWHERE   (field ILIKE '%word%')
   *  @li BOUNDARIES (field ILIKE 'word')
   *  @li BEGINNING  (field ILIKE 'word%')
   *  @li ENDING     (field ILIKE '%word')
   */
  enum SearchPattern {
    ANYWHERE = 0,   /**< @brief Anywhere, no word boundaries */
    BOUNDARIES = 1, /**< @brief Restrict to word boundaries */
    BEGINNING = 2,  /**< @brief Restrict to word beginning */
    ENDING = 3      /**< @brief Restrict to word ending */
  };

private:
  int p_minLength = 2;

private Q_SLOTS:
  void searchPatternChanged(int);

protected:
  const QRegularExpression jokerPattern = QRegularExpression("[\\%\\*]+");
  const QRegularExpression quotePattern = QRegularExpression("[\\'\\\"]+");
  const QRegularExpression trimPattern = QRegularExpression("[\\s\\t\\n\\r]+");
  const QRegularExpression isbnPattern = QRegularExpression("[^0-9]+");
  const QRegularExpression articlePattern =
      QRegularExpression("^([0-9]+[\\,]?)+$");

  /**
   * @brief Predefined with Stock CheckBox
   */
  QCheckBox *stockCheckBox(const QString &text = QString());

  /**
   * @brief Predefined Search Button
   */
  QPushButton *startSearchButton(const QString &text = QString());

  /**
   * @brief Predefined Search button
   */
  QPushButton *customSearchButton(const QString &text = QString());

  /**
   * @brief Search confiness settings
   */
  QComboBox *searchConfines();

  /**
   * @brief set pattern matches and turn around the search string.
   * @param field  - SQL Fieldname
   * @param search - Search string
   * @code
   *  In this Example code we search Authors with Forename and Surname.
   *  call:
   *    prepareFieldSearch("fieldname","forename surname")
   *
   *  returning:
   *    fieldname ILIKE "forename%surname" OR fieldname "surname%forename"
   *
   * @endcode
   * @return SQL Search Statement
   */
  const QString prepareFieldSearch(const QString &field, const QString &search);

protected Q_SLOTS:
  virtual void setSearch() = 0;
  virtual void setFilter(int) = 0;
  virtual void setFilterFocus() = 0;
  virtual void setClearAndFocus() = 0;
  virtual void setSearchFocus() = 0;

Q_SIGNALS:
  /**
   * @brief Minimum Search length has changed
   */
  void sendMinLengthChanged(int);

  /**
   * @brief With „Stock“ search enabled or not!
   */
  void sendStockEnabled(bool);

  /**
   * @brief Searchfilter has changed
   */
  void sendFilterChanged(int);

  /**
   * @brief will normally triggered inside of setSearch()
   */
  void sendSearchClicked();

  /**
   * @brief Send to Open a Custom Search Dialog
   * This is emitted by @ref customSearchButton()
   */
  void sendOpenCustomSearch();

  /**
   * @brief Search pattern chenged
   */
  void sendSearchPattern(TabsSearchBar::SearchPattern);

  /**
   * @brief send notification
   */
  void sendNotify(const QString &);

public Q_SLOTS:
  /**
   * @brief start search from minimal length
   */
  void setMinLength(int);

public:
  explicit TabsSearchBar(QWidget *parent = nullptr);

  /**
   * @brief get Icon from Theme
   */
  const QIcon getIcon(const QString &);

  /**
   * @brief present the selected Search syntax
   */
  TabsSearchBar::SearchPattern searchPattern() const;

  /**
   * @brief get start search from minimal length
   */
  int getMinLength();

  /**
   * @brief search only with stock
   */
  bool withStock();

  /**
   * @brief String length from current search input
   */
  virtual int searchLength() = 0;

  /**
   * @brief Prepared SQL Search statement
   */
  virtual const QString getSearchStatement() = 0;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_TABS_SEARCHBAR_H
