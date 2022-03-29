/** @COPYRIGHT_HOLDER@ */

#include "version.h"
#include "mapplication.h"
#include <QDir>
#include <QLocale>
#include <QTranslator>
#include <QByteArray>
#include <QLockFile>

int main(int argc, char *argv[])
{
    Antiqua a(argc, argv);
    a.setApplicationName ( ANTIQUACRM_NAME );
    a.setApplicationVersion ( ANTIQUACRM_VERSION_STRING );
    a.setApplicationDisplayName( ANTIQUACRM_NAME );
    a.setDesktopFileName( ANTIQUACRM_NAME);
    a.setOrganizationDomain ( HJCMSFQDN );

    // QByteArray hash(const QByteArray &data, QCryptographicHash::md5);
    // QLockFile lockFile(QDir::temp().absoluteFilePath(".lock"));

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "antiqua_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    return a.exec();
}
