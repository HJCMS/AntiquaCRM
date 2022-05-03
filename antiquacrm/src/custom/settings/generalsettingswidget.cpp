#include "generalsettingswidget.h"
#include "antiqua_global.h"
#include "applsettings.h"
#include "myicontheme.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

#include <QtWidgets/QTableWidget>

GeneralSettingsWidget::GeneralSettingsWidget(QWidget *parent)
    : SettingsWidget{parent} {
  setObjectName("general_config_widget");
  setWindowTitle(tr("General Configuration"));
  setSection("general");

  QString buffer; /**< Info Titel Puffer */
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("general_config_layout");

  // BEGIN
  QHBoxLayout *lay1 = new QHBoxLayout();
  buffer = tr("Image search Directory");
  m_editImageSrc = new LineEdit(this);
  m_editImageSrc->setPlaceholderText(tr("complete path to the main target"));
  m_editImageSrc->setInfo(buffer);
  lay1->addWidget(m_editImageSrc);

  QPushButton *btn_image_src = new QPushButton(this);
  btn_image_src->setIcon(myIcon("folder_green"));
  btn_image_src->setText(tr("Open Directory"));
  lay1->addWidget(btn_image_src);
  layout->addLayout(lay1);
  // END

  // BEGIN
  m_searchStart = new IntSpinBox(5, 20, this);
  m_searchStart->setObjectName("search/startlength");
  buffer = tr("From how many chars does the search start when you type in.");
  m_searchStart->setInfo(buffer);
  layout->addWidget(m_searchStart);
  // END

  // BEGIN
  m_minPrice = new IntSpinBox(5, 100, this);
  m_minPrice->setObjectName("books/min_price");
  buffer = tr("The lowest permissible selling price.");
  m_minPrice->setInfo(buffer);
  layout->addWidget(m_minPrice);
  // END

  layout->addStretch(1);
  setLayout(layout);
  connect(btn_image_src, SIGNAL(clicked()), this, SLOT(setImageDir()));
}

void GeneralSettingsWidget::setImageDir() {
  QVariant spath = config->value("imaging/sourcepath", QDir::homePath());
  QString src = getDirectory(spath.toString());
  if (src.isEmpty())
    return;

  m_editImageSrc->setValue(src);
}

void GeneralSettingsWidget::loadSectionConfig() {
  QVariant spath = config->value("imaging/sourcepath", QDir::homePath());
  m_editImageSrc->setValue(spath);
  m_editImageSrc->setToolTip(spath.toString());
  m_searchStart->setValue(config->value("search/startlength", 5));
  m_minPrice->setValue(config->value("books/min_price", 5));
}

void GeneralSettingsWidget::saveSectionConfig() {
  config->setValue("imaging/sourcepath", m_editImageSrc->value());
  config->setValue("search/startlength", m_searchStart->value());
  config->setValue("books/min_price", m_minPrice->value());
}
