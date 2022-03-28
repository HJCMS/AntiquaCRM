#include "antiquamainwindow.h"
#include "./ui_antiquamainwindow.h"

AntiquaMainWindow::AntiquaMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AntiquaMainWindow)
{
    ui->setupUi(this);
    connect( ui->actionConnect, SIGNAL ( triggered(bool) ),
             this, SLOT ( action_connect(bool) ) );
    connect( ui->actionClose, SIGNAL ( triggered(bool) ),
             this, SLOT ( action_closeandquit(bool) ) );
}

/**
 * @brief AntiquaMainWindow::action_connect
 * @param b
 * @short Wird von UI:QAction weitergeleited.
 */
void AntiquaMainWindow::action_connect(bool b)
{
    Q_UNUSED(b);
    emit psqlconnect();
}

/**
 * @brief AntiquaMainWindow::action_closeandquit
 * @param b
 * @short Wird von UI:QAction weitergeleited.
 */
void AntiquaMainWindow::action_closeandquit(bool b)
{
    Q_UNUSED(b);
    qDebug() << "TODO Save Quit Application";
}

AntiquaMainWindow::~AntiquaMainWindow()
{
    delete ui;
}
