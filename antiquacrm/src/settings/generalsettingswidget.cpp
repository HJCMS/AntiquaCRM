#include "generalsettingswidget.h"

#include <QtCore/QDebug>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QVBoxLayout>

#include <QTableWidget>

GeneralSettingsWidget::GeneralSettingsWidget(QWidget *parent)
    : SettingsWidget{parent}
{
    setObjectName("general_config_widget");
    setWindowTitle(tr("General Configuration"));
    setConfigSection("general");
    // tr("General Interface Settings.")

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("general_config_layout");

    QTableWidget* tb = new QTableWidget(this);
    mainLayout->addWidget(tb);

    setLayout(mainLayout);
}

void GeneralSettingsWidget::updateConfigSets(const QHash<QString,QVariant> &)
{

}

const QHash<QString,QVariant> & GeneralSettingsWidget::getSectionConfig()
{
    if(!p_hash.isEmpty())
        p_hash.clear();

    return p_hash;
}
