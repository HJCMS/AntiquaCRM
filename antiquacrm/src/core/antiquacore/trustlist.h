// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef TRUSTLIST_ANTIQUACORE_H
#define TRUSTLIST_ANTIQUACORE_H

#include "antiqua_global.h"

#include <QList>
#include <QString>
#include <QMetaType>

/**
 * @brief Trust SQL Field Class
 * @see c_trusted in public.costumers Table
 */
class ANTIQUACORE_EXPORT Trust {
private:
  int p_index;
  QString p_title;

public:
  explicit Trust();

  /**
   * @brief Trust
   * @param index Datafield Index
   * @param title DataField Translation
   */
  Trust(int index, const QString &title);

  /**
   * @brief Current Index
   */
  int index();

  /**
   * @brief Current Title
   */
  const QString title();
};
Q_DECLARE_TYPEINFO(Trust, Q_PRIMITIVE_TYPE);

/**
 * @class TrustList
 *
 * The TrustList class Provides Status from the
 * Trusted Indexes in current Database configuration.
 *
 * The "c_trusted" Field is a Type from smallint.
 * To your own Organise individual Types.
 * \ds costumers
 *      c_trusted | smallint
 *
 * @code
  #include <AntiquaCRM>

  TrustList list;
  int s = list.size();
  Trust myTrust(++s,tr("Personal trusted definition"));
  list.append(myTrust);

  QComboBox *m_box = new QComboBox(this);
  for (int i = 0; i < list.size(); i++)
  {
    Trust t = list.trust(i);
    m_box->insertItem(t.index(),t.title());
  }
 * @endcode
 */
class ANTIQUACORE_EXPORT TrustList : public QList<Trust> {
private:
  /**
   * @brief filled by Constructor
   */
  QList<Trust> p_list;

public:
  /**
   * @brief Initial and fills the TrustList
   */
  explicit TrustList();

  /**
   * @brief Current TrustList Size
   */
  int size();

  /**
   * @brief find Trust Object with Index
   */
  const Trust trust(int index);

  /**
   * @brief find Trust::index() with Index
   */
  const int index(int index);

  /**
   * @brief find Trust::title() with Index
   */
  const QString title(int index);
};
Q_DECLARE_TYPEINFO(TrustList, Q_COMPLEX_TYPE);

#endif // TRUSTLIST_ANTIQUACORE_H
