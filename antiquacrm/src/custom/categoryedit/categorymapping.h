// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CATEGORYEDIT_MAPPING_H
#define CATEGORYEDIT_MAPPING_H

#include <QList>
#include <QMetaType>
#include <QStringList>

/**
 * @brief Kategorien Zuweisung
 * @ingroup CategoryEdit
 */
class CategoryMapping {
private:
  int p_subid;
  QString p_sub;
  QStringList p_words;

public:
  explicit CategoryMapping();

  /**
   * @brief categories_extern.ce_binding
   */
  void setSubId(int subId);
  int getSubId();

  /**
   * @brief categories_extern.ce_name
   */
  void setSub(const QString &str);
  const QString getSub();

  /**
   * @brief categories_extern.ce_company_keywords
   */
  void setKeywords(const QStringList &list);
  const QStringList getKeywords();
};
typedef QList<CategoryMapping> CategoryMappingList;
Q_DECLARE_METATYPE(CategoryMapping);

#endif // CATEGORYEDIT_MAPPING_H
