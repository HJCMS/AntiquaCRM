/** @COPYRIGHT_HOLDER@ */

#include "version.h"
#include "applsettings.h"

#include <QtCore/QDebug>

ApplSettings::ApplSettings()
    : QSettings(HJCMSFQDN,ANTIQUACRM_NAME)
{}

const QHash<QString,QVariant> & ApplSettings::readGroupConfig(const QString &group)
{
    p_hash.clear();
    beginGroup(group);
    foreach (QString key,allKeys())
    {
       p_hash.insert(key,value(key));
    }
    endGroup();
    return p_hash;
}

void ApplSettings::writeGroupConfig(const QString &group, const QHash<QString,QVariant> &dataset)
{
    if(group.isEmpty())
        return;

    beginGroup(group);
    QHashIterator<QString,QVariant> i(dataset);
    while (i.hasNext())
    {
        i.next();
        setValue(i.key(),i.value());
        // qDebug() << __FUNCTION__ << i.key() << ": " << i.value() << Qt::endl;
    }
    endGroup();
}
