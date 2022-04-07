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
#define ANTIQUACRM_NAME "@PROJECTNAME@"
#define ANTIQUACRM_DISPLAYNAME "@DISPLAYNAME@"
#define ANTIQUACRM_VERSION "@ANTIQUACRM_VERSION_MAJOR@.@ANTIQUACRM_VERSION_MINOR@.@ANTIQUACRM_VERSION_RELEASE@"
#define ANTIQUACRM_VERSION_STRING "@ANTIQUACRM_VERSION_STRING@"
#define ANTIQUACRM_CONFIG_DOMAIN "hjcms.de"


static const QUrl AntiquaHomepage = QUrl("@HOMEPAGEURL@");

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
