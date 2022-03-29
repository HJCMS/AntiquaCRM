#ifndef ANTIQUACRM_VERSION_H
#define ANTIQUACRM_VERSION_H

#include <QtCore/QGlobalStatic>
#include <QtCore/QString>
#include <QtCore/QCryptographicHash>
#include <QtCore/QDir>
#include <QtCore/QByteArray>

/**
 * @short Project title settings
 */
#define ANTIQUACRM_NAME "@PROJECTNAME@"
#define ANTIQUACRM_DISPLAYNAME "@DISPLAYNAME@"
#define ANTIQUACRM_VERSION "@ANTIQUACRM_VERSION_MAJOR@.@ANTIQUACRM_VERSION_MINOR@.@ANTIQUACRM_VERSION_RELEASE@"
#define ANTIQUACRM_VERSION_STRING "@ANTIQUACRM_VERSION@"
#define HJCMSFQDN "hjcms.de"

/**
 * @short sqlConnectionName
 */
static QLatin1String sqlConnectionName = QLatin1String("ANTIQUA_SQL_CLIENT");

static const QString lockFilePath()
{
  QString sid(".lock");
  sid.prepend(ANTIQUACRM_NAME);
  sid.prepend("qtsingleapplication-");
  return QDir::temp().absoluteFilePath(sid);
}

#endif
