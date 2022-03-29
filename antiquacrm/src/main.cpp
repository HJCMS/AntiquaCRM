#include "antiquamainwindow.h"

#include <QDir>
#include <QLocale>
#include <QTranslator>
#include <QByteArray>
#include <QLockFile>
#include <antiqua.h>

int main(int argc, char *argv[])
{
    Antiqua a(argc, argv);
    a.setApplicationName ( "antiqua" );
    a.setApplicationVersion ( "0.0.1" );
    a.setApplicationDisplayName("Antiqua CRM");
    a.setDesktopFileName("Antiqua CRM");
    a.setOrganizationDomain ( "hjcms.de" );

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
