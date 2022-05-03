// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef @CLASSNAME@_H
#define @CLASSNAME@_H

#include "settingswidget.h"

#include <QtCore/QObject>
#include <QtWidgets>

class @CLASSNAME@ : public SettingsWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

public:
    explicit @CLASSNAME@(QWidget *parent = nullptr);

signals:

};

#endif // @CLASSNAME@_H
