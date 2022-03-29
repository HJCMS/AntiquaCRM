#include "searchtoolbar.h"

#include <QtCore/QDebug>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLayout>

SearchToolBar::SearchToolBar(QWidget * parent)
    : QToolBar(parent)
{
    setObjectName("SearchToolBar");
    setWindowTitle(tr("&Search"));
    setOrientation ( Qt::Horizontal );
    setAllowedAreas ( ( Qt::TopToolBarArea | Qt::BottomToolBarArea ) );

    QLabel* label = new QLabel (tr( "Title search"),this);
    label->setContentsMargins ( 5, 0, 5, 0 );
    addWidget ( label );

    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setObjectName("SearchEdit");
    addWidget(m_lineEdit);
    addSeparator();

    m_searchExact = new QCheckBox(tr("Exact search"),this);
    m_searchExact->setObjectName("SearchType");
    m_searchExact->setChecked(true);
    addWidget(m_searchExact);
    addSeparator();

    QPushButton *button = new QPushButton(tr("Search"),this);
    button->setObjectName("SearchButton");
    button->setContentsMargins ( 5, 0, 15, 0 );
    connect (button, SIGNAL ( pressed() ), this, SLOT ( startQuerySearch() ) );
    addWidget(button);
    addSeparator();
}

void SearchToolBar::startQuerySearch()
{
    qDebug() << m_lineEdit->text();
}
