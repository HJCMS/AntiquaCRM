// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "generalsettings.h"

#include <QtWidgets>

GeneralSettings::GeneralSettings(QWidget *parent) : SettingsWidget{parent} {
  setObjectName("general_settings");

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("general_config_layout");

  int row = 0; /**< Counter */
  QString buffer = QString();

  // BEGIN Applications
  QGroupBox *m_grouBox1 = new QGroupBox(this);
  m_grouBox1->setObjectName("groupbox_archives");
  m_grouBox1->setTitle(tr("Applications paths"));
  QGridLayout *lt_groupBox1 = new QGridLayout(m_grouBox1);
  // Mail
  m_mailler_path = new LineEdit(m_grouBox1);
  m_mailler_path->setObjectName("dirs/mailappl");
  m_mailler_path->setInfo(tr("eMail Application"));
  lt_groupBox1->addWidget(m_mailler_path, row, 0, 1, 1);
  btn_mailler = new QToolButton(m_grouBox1);
  btn_mailler->setIcon(getIcon("view_list"));
  btn_mailler->setText(tr("Open target"));
  lt_groupBox1->addWidget(btn_mailler, row++, 1, 1, 1);
  connect(btn_mailler, SIGNAL(clicked()), this, SLOT(setMailApplication()));
  m_grouBox1->setLayout(lt_groupBox1);
  layout->addWidget(m_grouBox1);
  buffer.clear();
  // END

  // BEGIN Payment
  QGroupBox *m_paymentGroup = new QGroupBox(this);
  m_paymentGroup->setObjectName("groupbox_payment");
  m_paymentGroup->setTitle(tr("Payment Settings"));
  QVBoxLayout *lt_payment = new QVBoxLayout(m_paymentGroup);
  // Preis
  buffer = tr("The lowest permissible selling price");
  m_minPrice = new IntSpinBox(5, 100, m_paymentGroup);
  m_minPrice->setObjectName("payment/min_price");
  m_minPrice->setInfo(buffer);
  lt_payment->addWidget(m_minPrice);
  buffer.clear();
  // Währung
  m_currency = new Ecurrency(m_paymentGroup);
  m_currency->setObjectName("payment/currency");
  m_currency->setInfo(tr("Currency for Displaying prices"));
  lt_payment->addWidget(m_currency);
  // Mehwertsteuer
  QHBoxLayout *vat_layout = new QHBoxLayout();
  m_vat1 = new IntSpinBox(m_paymentGroup);
  m_vat1->setObjectName("payment/vat1");
  m_vat1->setInfo(tr("VAT (normal)"));
  m_vat1->setRange(1, 50);
  m_vat1->setValue("19");
  m_vat1->setSuffix("%");
  vat_layout->addWidget(m_vat1);
  m_vat2 = new IntSpinBox(m_paymentGroup);
  m_vat2->setObjectName("payment/vat2");
  m_vat2->setInfo(tr("VAT (reduced)"));
  m_vat2->setRange(1, 50);
  m_vat2->setValue("7");
  m_vat2->setSuffix("%");
  vat_layout->addWidget(m_vat2);
  lt_payment->addLayout(vat_layout);
  // Zahlbar bis Tage
  m_payWaitDays = new IntSpinBox(m_paymentGroup);
  m_payWaitDays->setObjectName("payment/grace_period");
  m_payWaitDays->setInfo(tr("Payment grace period"));
  m_payWaitDays->setRange(10, 60);
  m_payWaitDays->setSuffix(" " + tr("Days"));
  lt_payment->addWidget(m_payWaitDays);
  m_paymentGroup->setLayout(lt_payment);
  layout->addWidget(m_paymentGroup);
  // END

  // BEGIN Extras
  row = 0; /**< Counter zurücksetzen */
  QGroupBox *m_grouBox2 = new QGroupBox(this);
  m_grouBox2->setObjectName("groupbox_additional");
  m_grouBox2->setTitle(tr("Additional"));
  QVBoxLayout *lt_groupBox2 = new QVBoxLayout(m_grouBox2);
  // Search
  buffer = tr("From how many chars does the search start when you type in.");
  m_searchStart = new IntSpinBox(5, 50, this);
  m_searchStart->setObjectName("search/startlength");
  m_searchStart->setInfo(buffer);
  lt_groupBox2->addWidget(m_searchStart);
  // Minimum Image Size
  QFrame *imageSizeFrame = new QFrame(m_grouBox2);
  QVBoxLayout *image_layout = new QVBoxLayout(imageSizeFrame);
  QString sizeinfo = tr("Set size range for Imagepreviews.");
  image_layout->addWidget(new QLabel(sizeinfo, imageSizeFrame));
  m_minSize = new SizeEdit(imageSizeFrame);
  m_minSize->setObjectName("image/min_size");
  m_minSize->setMinimum(80, 80);
  m_minSize->setMaximum(800, 800);
  m_minSize->setValue(QSize(125, 125));
  m_minSize->setInfo(tr("Minimum Image size"));
  image_layout->addWidget(m_minSize);
  // Maximum Image Size
  m_maxSize = new SizeEdit(imageSizeFrame);
  m_maxSize->setObjectName("image/max_size");
  m_maxSize->setMinimum(80, 80);
  m_maxSize->setMaximum(800, 800);
  m_maxSize->setValue(QSize(320, 320));
  m_maxSize->setInfo(tr("Maximum Image size"));
  image_layout->addWidget(m_maxSize);
  imageSizeFrame->setLayout(image_layout);
  lt_groupBox2->addWidget(imageSizeFrame);
  m_grouBox2->setLayout(lt_groupBox2);
  layout->addWidget(m_grouBox2);
  // END

  // BEGIN
  row = 0; /**< Counter zurücksetzen */
  QGroupBox *m_grouBox3 = new QGroupBox(this);
  m_grouBox3->setObjectName("groupbox_additional");
  m_grouBox3->setTitle(tr("Font Settings"));
#ifdef Q_OS_LINUX
  m_grouBox3->setToolTip(tr("On GNU/Linux use the System Settings!"));
  m_grouBox3->setEnabled(false);
#endif
  QGridLayout *lt_groupBox3 = new QGridLayout(m_grouBox3);
  // Font Settings
  view_font_config = new QLabel(this);
  view_font_config->setText(tr("fonts representation"));
  lt_groupBox3->addWidget(view_font_config, row++, 0, 1, 1);
  buffer = tr("open font settings");
  btn_fontdialog = new QPushButton(buffer, m_grouBox3);
  btn_fontdialog->setIcon(getIcon("view_text"));
  lt_groupBox3->addWidget(btn_fontdialog, row++, 1, 1, 1, Qt::AlignLeft);
  layout->addWidget(m_grouBox3);
  connect(btn_fontdialog, SIGNAL(clicked()), this, SLOT(openFontDialog()));
  // END

  // Final
  layout->addStretch(1);
  setLayout(layout);

  initSignalChanged();
}

void GeneralSettings::initSignalChanged() {
  QList<InputEdit *> l =
      findChildren<InputEdit *>(QString(), Qt::FindChildrenRecursively);
  if (l.count() > 1) {
    for (int i = 0; i < l.count(); i++) {
      InputEdit *w = l.at(i);
      if (w != nullptr) {
        connect(w, SIGNAL(sendHasModified(bool)), this,
                SLOT(chieldModified(bool)));
      }
    }
  }
}

void GeneralSettings::openFontDialog() {
  bool b = true;
  QFont font = QFontDialog::getFont(&b, qApp->font(), this);
  if (b) {
    view_font_config->setFont(font);
    chieldModified(true);
  }
}

void GeneralSettings::setMailApplication() {
  QVariant spath = config->value("dir/mailappl", QDir::rootPath());
  QString src = QFileDialog::getOpenFileName(this,      // Main
                                             "Mailler", // Caption
                                             spath.toString());
  QFileInfo info(src);
  if (info.isExecutable()) {
    m_mailler_path->setValue(src);
  }
}

void GeneralSettings::setPageTitle(const QString &title) {
  pageTitle = title;
  emit pageTitleChanged();
}

const QString GeneralSettings::getPageTitle() { return pageTitle; }

void GeneralSettings::setPageIcon(const QIcon &icon) {
  pageIcon = icon;
  emit pageIconChanged();
}

const QIcon GeneralSettings::getPageIcon() { return pageIcon; }

void GeneralSettings::loadSectionConfig() {
  QList<InputEdit *> l =
      findChildren<InputEdit *>(QString(), Qt::FindChildrenRecursively);
  if (l.count() > 1) {
    for (int i = 0; i < l.count(); i++) {
      InputEdit *w = l.at(i);
      if (w != nullptr && config->contains(w->objectName())) {
        w->setValue(config->value(w->objectName()));
      }
    }
  }
  QFont font = qApp->font();
  if (!font.fromString(config->value("font", font.toString()).toString())) {
    font.setFamily(config->value("font_family").toString());
    font.setPointSize(config->value("font_size", font.pointSize()).toInt());
  }
  view_font_config->setFont(font);
  view_font_config->setText(tr("fonts representation"));
}

void GeneralSettings::saveSectionConfig() {
  /**
   * @list Müssen noch eingfügt werden!
   * @li PopUp-Fenster-Timeout in Sekunden "popup_timeout" Standard: 2
   * @{
   */
  config->setValue("popup_timeout", 2);
  config->setValue("mouse_wheel_actions", false);
  /**< @} */

  QFont font = view_font_config->font();
  config->setValue("font", font.toString());
  config->setValue("font_size", font.pointSize());
  config->setValue("font_family", font.family());

  QList<InputEdit *> l =
      findChildren<InputEdit *>(QString(), Qt::FindChildrenRecursively);
  if (l.count() > 1) {
    for (int i = 0; i < l.count(); i++) {
      InputEdit *w = l.at(i);
      if (w != nullptr) {
        config->setValue(w->objectName(), w->value());
      }
    }
  }
}
