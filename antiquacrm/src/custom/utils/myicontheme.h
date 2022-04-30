// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef MYICONTHEME_UTILS_H
#define MYICONTHEME_UTILS_H

#include <QtCore/QObject>
#include <QtGui/QIcon>
#include <QtGui/QIconEngine>

/**
 * @todo MyIconTheme class
 */
class MyIconTheme : public QIconEngine {
public:
  MyIconTheme();
};

/**
 * @brief myIcon
 * @param name - Name without Extension and Path
 */
static const QIcon myIcon(const QString &name) {
  QString resourceFile(":icons/");
  resourceFile.append(name);
  resourceFile.append(".png");
  return QIcon(resourceFile);
}

#endif // MYICONTHEME_UTILS_H
