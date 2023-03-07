// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SEARCHFILTER_H
#define ANTIQUACRM_SEARCHFILTER_H

#include <QJsonObject>
#include <QList>
#include <QMetaType>
#include <QRegExp>
#include <QRegularExpression>
#include <QString>

/**
 * @brief The SearchFilter class
 * @ingroup widgets
 */
class SearchFilter {
private:
  QRegExp p_cleanInput;
  QStringList p_fieldset;
  QString p_title;
  QString p_search;
  int p_type;

  const QRegExp defaultPattern() const;
  const QRegularExpression pattern() const;

public:
  enum SearchType {
    STRINGS = 0,   /**< @brief String Request */
    NUMERIC = 1,   /**< @brief Numeric Request */
    REFERENCES = 2 /**< @brief Reference Request */
  };
  explicit SearchFilter();
  SearchFilter(const SearchFilter &other);

  /**
   * @brief Titel in der Auswahlbox
   */
  void setTitle(const QString &);
  QString getTitle();

  /**
   * @brief Welche Felder werden abgefragt
   */
  void setFields(const QString &f);
  void setFields(const QStringList &f);
  QStringList getFields();

  /**
   * @brief Suchanfrage der Benutzereingabe
   */
  void setSearch(const QString &s);
  QString getSearch();

  /**
   * @brief Art der Validierung
   */
  void setType(SearchFilter::SearchType t);
  bool setType(int t);
  SearchFilter::SearchType getType();

  /**
   * @brief Komplette Filter Beschreibung in einem Variant
   */
  QJsonObject getFilter();
};

#endif // ANTIQUACRM_SEARCHFILTER_H
