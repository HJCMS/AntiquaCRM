#include "generalsettingswidget.h"
#include "applsettings.h"
#include "version.h"

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
  setConfigSection("general");

  ApplSettings p_cfg(this); /**< Konfigurationen laden */

  QSpacerItem *verticalSpacer =
      new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

  QSpacerItem *horizontalSpacer =
      new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Minimum);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setObjectName("general_config_layout");

  QGridLayout *gridLayout = new QGridLayout();

  // Layout
  // |--0--|--1--|--2--|--3--|
  // |---------Label---------|
  // |----LinieEdit----|-Btn-|
  // |--Label+SpinBox+Spacer-|
  //
  QLabel *ml1 = new QLabel(this);
  ml1->setText(tr("Main Location from Image source Directory."));
  gridLayout->addWidget(ml1, 0, 0, 1, 3);

  //BEGIN#1
  if (!p_cfg.contains("imaging/sourcepath"))
    p_cfg.setValue("imaging/sourcepath", QDir::homePath());

  m_editImageSrc = new QLineEdit(this);
  m_editImageSrc->setObjectName("imaging/sourcepath");
  m_editImageSrc->setPlaceholderText(tr("complete path to the main target"));
  m_editImageSrc->setMinimumWidth(200);
  gridLayout->addWidget(m_editImageSrc, 1, 0, 1, 2);

  QPushButton *m_btn = new QPushButton(this);
  m_btn->setIcon(myIcon("folder_green"));
  m_btn->setText(tr("Open Directory"));
  gridLayout->addWidget(m_btn, 1, 2, 1, 1);
  //END#1

  //BEGIN#2
  QHBoxLayout *hl2 = new QHBoxLayout();
  QLabel *ml2 = new QLabel(this);
  ml2->setText(
      tr("From how many characters does the search start when you type in."));
  ml2->setWordWrap(true);
  ml2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
  hl2->addWidget(ml2);

  if (!p_cfg.contains("search/startlength"))
    p_cfg.setValue("search/startlength", 5);

  m_searchStart = new QSpinBox(this);
  m_searchStart->setObjectName("search/startlength");
  m_searchStart->setRange(5, 20);
  m_searchStart->setValue(p_cfg.value("search/startlength", 5).toInt());
  hl2->addWidget(m_searchStart);
  hl2->addSpacerItem(horizontalSpacer);
  gridLayout->addLayout(hl2, 2, 0, 1, 3);
  //END#2

  //BEGIN#3
  if (!p_cfg.contains("books/min_price"))
        p_cfg.setValue("books/min_price", 8.00);

  qDebug() << "TODO BOOKD MIN PRICE";
  //END#3

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
