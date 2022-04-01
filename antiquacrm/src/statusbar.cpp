#include "statusbar.h"

#include <QtCore/QDebug>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>

StatusBar::StatusBar(QWidget * parent)
    : QStatusBar{parent}
{
    setObjectName("StatusBar");
/*
    QLabel* m_dbInfo = new QLabel(this);
    m_dbInfo->setText("Connection");
    addPermanentWidget(m_dbInfo,1);
*/
}

void StatusBar::sqlStatusMessage(const QString &str)
{
    qDebug() << __FUNCTION__ << Qt::endl;
    showMessage(str,2);
}

StatusBar::~StatusBar()
{}
