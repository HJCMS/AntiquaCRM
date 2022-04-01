// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#include "version.h"
#include "mapplication.h"

#include <QtCore/QLocale>
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QTranslator>

int main(int argc, char *argv[])
{
    /**
     * @TODO Create Sinlge Application
     */
    if(QFile::exists(lockFilePath()))
    {
        qInfo("Allready Running.");
        return 0;
    }

    MApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "antiquacrm_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    return a.exec();
}
