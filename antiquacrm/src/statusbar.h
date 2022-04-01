#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QtCore/QObject>
#include <QtWidgets/QWidget>
#include <QtWidgets/QStatusBar>

class StatusBar : public QStatusBar
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

public Q_SLOTS:
    void sqlStatusMessage(const QString &);

public:
    explicit StatusBar(QWidget * parent = 0);
    ~StatusBar();

};

#endif // STATUSBAR_H
