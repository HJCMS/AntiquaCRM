#ifndef SEARCHTOOLBAR_H
#define SEARCHTOOLBAR_H

/* QtCore */
#include <QtCore/QObject>
/* QtWidgets */
#include <QtWidgets/QWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QCheckBox>

class SearchToolBar : public QToolBar
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

private:
    QLineEdit *m_lineEdit;
    QCheckBox *m_searchExact;

private Q_SLOTS:
    void startQuerySearch();

// Q_SIGNALS:

public:
    explicit SearchToolBar(QWidget * main);
};

#endif // SEARCHTOOLBAR_H
