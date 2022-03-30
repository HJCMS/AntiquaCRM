/** @COPYRIGHT_HOLDER@ */

#ifndef ANTIQUA_APPLSETTINGS_H
#define ANTIQUA_APPLSETTINGS_H

#include <QtCore/QHash>
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

class ApplSettings : public QSettings
{
private:
    QHash<QString,QVariant> p_hash;

public:
    explicit ApplSettings();
    void writeGroupConfig(const QString &,const QHash<QString,QVariant> &);
    const QHash<QString,QVariant> & readGroupConfig(const QString &);
};

#endif // ANTIQUA_APPLSETTINGS_H
