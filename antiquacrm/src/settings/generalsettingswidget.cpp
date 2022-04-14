#include "generalsettingswidget.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

#include <QTableWidget>

GeneralSettingsWidget::GeneralSettingsWidget(QWidget *parent)
    : SettingsWidget{parent} {
  setObjectName("general_config_widget");
  setWindowTitle(tr("General Configuration"));
  setConfigSection("general");
  // tr("General Interface Settings.")

  QSpacerItem *verticalSpacer =
      new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
  // QSpacerItem *horizontalSpacer = new
  // QSpacerItem(10,10,QSizePolicy::Minimum,QSizePolicy::Expanding);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setObjectName("general_config_layout");

  QGridLayout *gridLayout = new QGridLayout();

  QLabel *ml1 = new QLabel(this);
  ml1->setText(tr("Main Location from Image source Directory."));
  gridLayout->addWidget(ml1, 0, 0, 1, 3);

  m_editImageSrc = new QLineEdit(this);
  m_editImageSrc->setObjectName("image_source_path");
  m_editImageSrc->setPlaceholderText(tr("complete path to the main target"));
  gridLayout->addWidget(m_editImageSrc, 1, 0, 1, 1);

  QPushButton *m_btn = new QPushButton(this);
  m_btn->setIcon(myIcon("folder_green"));
  m_btn->setText(tr("Open Directory"));
  gridLayout->addWidget(m_btn, 1, 1, 1, 1);

  mainLayout->addLayout(gridLayout);

  mainLayout->addSpacerItem(verticalSpacer);
  setLayout(mainLayout);
}

void GeneralSettingsWidget::updateConfigSets(const QHash<QString, QVariant> &) {

}

const QHash<QString, QVariant> &GeneralSettingsWidget::getSectionConfig() {
  if (!p_hash.isEmpty())
    p_hash.clear();

  return p_hash;
}
