/** @COPYRIGHT_HOLDER@ */

#ifndef ANTIQUACRM_MAPPLICATION_H
#define ANTIQUACRM_MAPPLICATION_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QEvent>
/* QtWidgets */
#include <QtWidgets/QApplication>
/* QtSQL */
#include <QtSql/QSqlDatabase>
/* Project */
#include "mwindow.h"

class MApplication : public QApplication
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

private:
    MWindow *m_mainWindow;
    void initSettings();
    bool initDatabase();

private Q_SLOTS:
    void connectdb();

Q_SIGNALS:
    void sqlconnect();

public:
    QSqlDatabase *m_db;
    explicit MApplication(int &argc, char **argv);
    int exec();
    ~MApplication();
};

#endif // ANTIQUACRM_H
