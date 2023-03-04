// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TABSEARCHBAR_H
#define ANTIQUACRM_TABSEARCHBAR_H

#include <QCheckBox>
#include <QComboBox>
#include <QObject>
#include <QPushButton>
#include <QRegExp>
#include <QRegExpValidator>
#include <QString>
#include <QToolBar>
#include <QWidget>

class TabSearchBar : public QToolBar {
  Q_OBJECT
  Q_PROPERTY(int minLength READ getMinLength WRITE setMinLength NOTIFY
                 sendMinLengthChanged)

public:
  /**
   * @brief settings confiness for search syntax
   * @list Enumeration to restrict search to:
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
  int minLength = 2;

private Q_SLOTS:
  void setSearchStockEnabled(bool);
  void setSearchPatternChanged(int);

protected:
  const QRegExp jokerPattern = QRegExp("[%*]+");
  const QRegExp quotePattern = QRegExp("[\\'\\\"]+");
  const QRegExp trimPattern = QRegExp("[\\s\\t\\n\\r]+");
  const QRegExp isbnPattern = QRegExp("[^0-9]+");
  const QRegExp articlePattern = QRegExp("^([0-9]+[\\,]?)+$");

  /**
   * @brief is Stocksearch Enabled?
   */
  bool SearchWithStock = false;

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

  const QString prepareFieldSet(const QString &fieldname,
                                const QString &search);

protected Q_SLOTS:
  virtual void setSearch() = 0;
  virtual void setFilter(int) = 0;
  virtual void setFilterFocus() = 0;
  virtual void setClearAndFocus() = 0;
  virtual void setSearchFocus() = 0;

Q_SIGNALS:
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
   * @brief will normally triggered inside of @ref setSearch()
   */
  void sendSearchClicked();

  /**
   * @brief Send to Open a Custom Search Dialog
   * This is emitted by @ref customSearchButton()
   */
  void sendOpenCustomSearch();

  void sendMatchChanged(TabSearchBar::SearchPattern);

  void sendNotify(const QString &);

public Q_SLOTS:
  /**
   * @brief start search from minimal length
   */
  void setMinLength(int);

public:
  explicit TabSearchBar(QWidget *parent = nullptr);

  /**
   * @brief present the selected Search syntax
   */
  TabSearchBar::SearchPattern currentSearchSyntax() const;

  /**
   * @brief get start search from minimal length
   */
  int getMinLength();

  /**
   * @brief String length from current search input
   */
  virtual int searchLength() = 0;

  /**
   * @brief Prepared SQL Search statement
   */
  virtual const QString getSearchStatement() = 0;
};

#endif // ANTIQUACRM_TABSEARCHBAR_H
