#include "antiquamainwindow.h"

#include <QtCore/QLocale>
#include <QtCore/QTranslator>

#include <antiqua.h>

int main(int argc, char *argv[])
{
    Antiqua a(argc, argv);
    a.setApplicationName ( "antiqua" );

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
