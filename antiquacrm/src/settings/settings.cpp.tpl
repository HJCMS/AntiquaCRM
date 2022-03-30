#include "@CLASSNAME@.h"

#include <QtCore/QDebug>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QVBoxLayout>

@CLASSNAME@::@CLASSNAME@(QWidget *parent)
    : SettingsWidget{parent}
{
    setObjectName("@CLASSNAME@");
  /* Required  Settings
    setConfigSection("");
    setWindowTitle(tr(""));
    setConfigTitle(tr(""));
  */
}

