// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DESIGNATIONS_DATA_H
#define DESIGNATIONS_DATA_H

#include <QMetaType>
#include <QString>

/**
 * @brief The DesignationData class
 * @ingroup utils
 */
struct DesignationData {
  int id = -1;
  QString keyword;
  QString description;
};

Q_DECLARE_METATYPE(DesignationData);

#endif // DESIGNATIONS_DATA_H
