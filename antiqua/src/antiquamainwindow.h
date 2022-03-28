#ifndef ANTIQUAMAINWINDOW_H
#define ANTIQUAMAINWINDOW_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QEvent>
/* QtWidgets */
#include <QtWidgets/QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class AntiquaMainWindow; }
QT_END_NAMESPACE

class AntiquaMainWindow : public QMainWindow
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

private:
    Ui::AntiquaMainWindow *ui;

private Q_SLOTS:
    void action_connect(bool);
    void action_closeandquit(bool);

Q_SIGNALS:
    void psqlconnect();

public:
    AntiquaMainWindow(QWidget *parent = nullptr);
    ~AntiquaMainWindow();

};
#endif // ANTIQUAMAINWINDOW_H
