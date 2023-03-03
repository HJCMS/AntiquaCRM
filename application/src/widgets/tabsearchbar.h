// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TABSEARCHBAR_H
#define ANTIQUACRM_TABSEARCHBAR_H

#include <QCheckBox>
#include <QObject>
#include <QPushButton>
#include <QRegExp>
#include <QRegExpValidator>
#include <QString>
#include <QToolBar>
#include <QWidget>

class TabSearchBar : public QToolBar {
  Q_OBJECT

private Q_SLOTS:
  void setSearchStockEnabled(bool);

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

protected Q_SLOTS:
  virtual void setSearch() = 0;
  virtual void setFilter(int) = 0;
  virtual void setFilterFocus() = 0;
  virtual void setClearAndFocus() = 0;
  virtual void setSearchFocus() = 0;

Q_SIGNALS:
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

public:
  explicit TabSearchBar(QWidget *parent = nullptr);

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
