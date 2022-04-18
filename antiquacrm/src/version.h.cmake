#ifndef ANTIQUACRM_VERSION_H
#define ANTIQUACRM_VERSION_H

#include <QtCore/QGlobalStatic>
#include <QtCore/QUrl>
#include <QtCore/QString>
#include <QtCore/QCryptographicHash>
#include <QtCore/QDir>
#include <QtCore/QByteArray>
#include <QtCore/QMap>
#include <QtGui/QIcon>

/**
 * @short Project title settings
 */
#ifndef ANTIQUACRM_NAME
#define ANTIQUACRM_NAME "@PROJECTNAME@"
#endif

#ifndef ANTIQUACRM_DISPLAYNAME
#define ANTIQUACRM_DISPLAYNAME "@DISPLAYNAME@"
#endif

#ifndef ANTIQUACRM_VERSION
#define ANTIQUACRM_VERSION "@ANTIQUACRM_VERSION_MAJOR@.@ANTIQUACRM_VERSION_MINOR@.@ANTIQUACRM_VERSION_RELEASE@"
#endif

#ifndef ANTIQUACRM_VERSION_STRING
#define ANTIQUACRM_VERSION_STRING "@ANTIQUACRM_VERSION_STRING@"
#endif

#ifndef HJCMS_CONFIG_DOMAIN
#define HJCMS_CONFIG_DOMAIN "de.hjcms"
#endif

/**
* @brief System Hostname must append to this ...
*/
#ifndef ANTIQUACRM_CONNECTION_PREFIX
#define ANTIQUACRM_CONNECTION_PREFIX "de.hjcms."
#endif

#ifndef ANTIQUACRM_HOMEPAGE
#define ANTIQUACRM_HOMEPAGE "@HOMEPAGEURL@"
#endif

/**
 * @brief myIcon
 * @param name - Name without Extension and Path
 */
static const QIcon myIcon(const QString &name)
{
  QString resourceFile(":icons/");
  resourceFile.append(name);
  resourceFile.append(".png");
  return QIcon(resourceFile);
}

#endif
