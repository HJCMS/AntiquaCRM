// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef TRUSTLIST_ANTIQUA_CORE_H
#define TRUSTLIST_ANTIQUA_CORE_H

#include <QtCore/QList>
#include <QtCore/QMetaType>

class Trust;

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
  Trust myItem(++s,tr("Personal trusted definition"));
  list.append(Trust);

  QComboBox *m_box = new QComboBox(this);
  for (int i = 0; i < list.size(); i++)
  {
    Trust t = list.trust(i);
    m_box->insertItem(t.index(),t.title());
  }
 * @endcode
 */
QT_SHARED class TrustList : public QList<Trust> {
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
  const int trustIndex(int index);

  /**
   * @brief find Trust::title() with Index
   */
  const QString trustTitle(int index);
};

Q_DECLARE_TYPEINFO(TrustList, Q_COMPLEX_TYPE);

#endif // TRUSTLIST_ANTIQUA_CORE_H
