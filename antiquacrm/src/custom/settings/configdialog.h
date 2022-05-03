/** @COPYRIGHT_HOLDER@ */

#ifndef ANTIQUA_CONFIGDIALOG_H
#define ANTIQUA_CONFIGDIALOG_H

#include "applsettings.h"
#include "settingswidget.h"
#include "generalsettingswidget.h"
#include "postgresqlsettings.h"

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QStackedLayout>
#include <QListWidget>
#include <QListWidgetItem>

class ConfigDialog : public QDialog
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

private:
    ApplSettings* m_settings;
    QStackedLayout* m_pageSet;
    QListWidget* m_listWidget;
    GeneralSettingsWidget* m_page1;
    PostgreSqlSettings* m_page2;
    void createItemSelection();

private Q_SLOTS:
    void setPage(QListWidgetItem *);

public:
    explicit ConfigDialog(QWidget *parent = nullptr);

public Q_SLOTS:
    void saveConfig();
    void closeDialog();

};

#endif // ANTIQUA_CONFIGDIALOG_H
