#ifndef ANTIQUA_H
#define ANTIQUA_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QEvent>
/* QtWidgets */
#include <QtWidgets/QApplication>
/* QtSQL */
#include <QtSql/QSqlDatabase>
/* Project */
#include "amainwindow.h"

class Antiqua : public QApplication
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

private:
    AMainWindow *m_mainWindow;
    void initSettings();
    bool initDatabase();

private Q_SLOTS:
    void connectdb();

Q_SIGNALS:
    void sqlconnect();

public:
    QSqlDatabase *m_db;
    explicit Antiqua(int &argc, char **argv);
    int exec();
    ~Antiqua();
};

#endif // ANTIQUA_H
