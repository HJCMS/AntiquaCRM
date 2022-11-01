// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TABSEARCHBAR_H
#define ANTIQUACRM_TABSEARCHBAR_H

#include <QObject>
#include <QPushButton>
#include <QRegExp>
#include <QRegExpValidator>
#include <QString>
#include <QToolBar>
#include <QWidget>

class TabSearchBar : public QToolBar {
  Q_OBJECT

protected:
  const QRegExp jokerPattern = QRegExp("[%*]+");
  const QRegExp quotePattern = QRegExp("[\\'\\\"]+");
  const QRegExp trimPattern = QRegExp("[\\s\\t\\n\\r]+");
  const QRegExp isbnPattern = QRegExp("[^0-9]+");
  const QRegExp articlePattern = QRegExp("^([0-9]+[\\,]?)+$");

  /**
   * @brief Predefined Search Button
   */
  QPushButton *clickSearchButton(const QString &text);

protected Q_SLOTS:
  virtual void setSearch() = 0;
  virtual void setFilter(int) = 0;
  virtual void setFilterFocus() = 0;
  virtual void setClearAndFocus() = 0;
  virtual void setSearchFocus() = 0;

Q_SIGNALS:
  /**
   * @brief Searchfilter has changed
   */
  void sendFilterChanged(int index);

  /**
   * @brief will normally triggered inside of @ref setSearch()
   */
  void sendSearchClicked();

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
