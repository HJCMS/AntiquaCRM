/** @COPYRIGHT_HOLDER@ */

#include "antiqua_global.h"
#include "configdialog.h"
#include "myicontheme.h"

#include <QtCore/QDebug>
#include <QtCore/QVariant>
#include <QtCore/QStringList>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

ConfigDialog::ConfigDialog(QWidget *parent) : QDialog(parent)
{
    setObjectName("ConfigDialog");
    setWindowTitle(tr("Configuration"));
    setSizeGripEnabled(true);
    setMinimumSize(800, 600);

    m_settings = new ApplSettings();

    QVBoxLayout *m_vLayout = new QVBoxLayout(this);
    m_vLayout->setObjectName("vlayout");

    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setObjectName("hlayout");

    m_pageSet = new QStackedLayout();
    m_pageSet->setObjectName("CentralLayout");
    hLayout->addLayout(m_pageSet,Qt::AlignLeft);

    m_page1 = new GeneralSettingsWidget(this);
    m_pageSet->addWidget(m_page1);

    m_page2 = new PostgreSqlSettings(this);
    m_pageSet->addWidget(m_page2);

    m_vLayout->addLayout(hLayout);

    m_listWidget = new QListWidget(this);
    m_listWidget->setObjectName("ListConfigWidget");
    m_listWidget->setResizeMode(QListView::Adjust);
    m_listWidget->setSortingEnabled(false);
    m_listWidget->setMinimumWidth(100);
    m_listWidget->setMaximumWidth(150);
    hLayout->addWidget(m_listWidget,Qt::AlignLeft);

    QDialogButtonBox *m_btnBox = new QDialogButtonBox(this);
    m_btnBox->setObjectName("ButtonBox");
    m_btnBox->setOrientation(Qt::Horizontal);
    m_btnBox->setStandardButtons(QDialogButtonBox::Save|QDialogButtonBox::Close);
    connect (m_btnBox, SIGNAL( accepted() ), this, SLOT( saveConfig() ) );
    connect (m_btnBox, SIGNAL( rejected() ), this, SLOT( closeDialog() ) );

    m_vLayout->addWidget(m_btnBox);

    setLayout(m_vLayout);

    createItemSelection();

    readWidgetConfig();
}

/**
 * @brief ConfigDialog::createItemSelection
 */
void ConfigDialog::createItemSelection()
{
    QListWidgetItem *m1 = new QListWidgetItem("ui_settings", m_listWidget);
    m1->setText(tr("Generally"));
    m1->setIcon(myIcon("list"));
    m_listWidget->addItem(m1);

    QListWidgetItem *m2 = new QListWidgetItem("sql_settings", m_listWidget);
    m2->setText(tr("Database"));
    m2->setIcon(myIcon("database"));
    m_listWidget->addItem(m2);

    QListWidgetItem *m3 = new QListWidgetItem("isbn_settings", m_listWidget);
    m3->setText(tr("OpenLibray"));
    m3->setIcon(myIcon("autostart"));
    m_listWidget->addItem(m3);

    connect (m_listWidget, SIGNAL ( itemDoubleClicked(QListWidgetItem*) ),
           this, SLOT( setPage(QListWidgetItem*) ));
}

void ConfigDialog::setPage(QListWidgetItem *item)
{
    int page = (m_listWidget->currentRow());
    if(m_pageSet->itemAt(page) != nullptr)
    {
        m_pageSet->setCurrentIndex(page);
    }
}

void ConfigDialog::readWidgetConfig()
{
    // PostgreSQL
    QHash<QString,QVariant> d = m_settings->readGroupConfig(m_page2->configSection());
    if(!d.isEmpty())
    {
        m_page2->updateConfigSets(d);
        return;
    }
}

void ConfigDialog::saveConfig()
{
    // GeneralSettings
    /*
    m_settings->writeGroupConfig(m_page1->configSection(),
                                 m_page1->getSectionConfig());
    */
    // PostgreSqlSettings
    m_settings->writeGroupConfig(m_page2->configSection(),
                                 m_page2->getSectionConfig());
}

void ConfigDialog::closeDialog()
{
    accept();
}
