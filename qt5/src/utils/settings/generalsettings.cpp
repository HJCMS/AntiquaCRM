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
  m_grouBox1->setTitle(tr("Applications paths") + "  ");
  QGridLayout *lt_groupBox1 = new QGridLayout(m_grouBox1);
  // Mail
  m_mailler_path = new LineEdit(m_grouBox1);
  m_mailler_path->setObjectName(ANTIQUACRM_CONFIG_MAILLER_KEY);
  m_mailler_path->setInfo(tr("eMail Application"));
  m_mailler_path->setWhatsThis(
      tr("Setting the Binary path to you favorite Mailler application. "
         "Currently only Thunderbird and Outlook supported."));
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
  m_paymentGroup->setTitle(tr("Payment Settings") + "  ");
  QVBoxLayout *lt_payment = new QVBoxLayout(m_paymentGroup);
  // payment/min_price
  buffer = tr("The lowest permissible selling price");
  m_minPrice = new PriceEdit(5, 20, m_paymentGroup);
  m_minPrice->setObjectName("payment/min_price");
  m_minPrice->setInfo(buffer);
  m_minPrice->setWhatsThis(
      tr("Here you can set a minimum price that must not be undercut."));
  lt_payment->addWidget(m_minPrice);
  buffer.clear();
  // payment/min_price_media
  buffer = tr("The lowest permissible multimedia selling price");
  m_minPriceMedia = new PriceEdit(1, 20, m_paymentGroup);
  m_minPriceMedia->setObjectName("payment/min_price_media");
  m_minPriceMedia->setInfo(buffer);
  lt_payment->addWidget(m_minPriceMedia);
  buffer.clear();
  // payment/default_price_cds
  buffer = tr("Default retail price for CDs.");
  m_defaultCDPrice = new PriceEdit(1, 50, m_paymentGroup);
  m_defaultCDPrice->setObjectName("payment/default_cd_price");
  m_defaultCDPrice->setInfo(buffer);
  lt_payment->addWidget(m_defaultCDPrice);
  buffer.clear();
  // payment/currency
  buffer = tr("Currency for Displaying prices");
  m_currency = new Ecurrency(m_paymentGroup);
  m_currency->setObjectName("payment/currency");
  m_currency->setInfo(buffer);
  lt_payment->addWidget(m_currency);
  buffer.clear();
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
  m_payWaitDays->setWhatsThis(
      tr("In this Section you can set the the Payment grace period."));
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
  m_grouBox2->setTitle(tr("Look and behavior") + "  ");
  QVBoxLayout *lt_groupBox2 = new QVBoxLayout(m_grouBox2);
  // [search] startlength
  buffer = tr("From how many chars does the search start when you type in.");
  m_searchStart = new IntSpinBox(3, 50, this);
  m_searchStart->setObjectName("search/startlength");
  m_searchStart->setWhatsThis(
      tr("From how many characters in the search input should the automatic "
         "search start? Warning - Values that are too low can slow down the "
         "program!"));
  m_searchStart->setInfo(buffer);
  lt_groupBox2->addWidget(m_searchStart);
  // [General] transfer_timeout
  buffer = tr("Timeout settings for network connections");
  m_transfer_timeout = new IntSpinBox(5, 30, this);
  m_transfer_timeout->setObjectName("transfer_timeout");
  m_transfer_timeout->setValue(QVariant(5));
  m_transfer_timeout->setInfo(buffer);
  lt_groupBox2->addWidget(m_transfer_timeout);
  // [General] popup_timeout
  buffer = tr("Timeout for Messenger PopUp Windows");
  m_popup_timeout = new IntSpinBox(1, 5, this);
  m_popup_timeout->setObjectName("popup_timeout");
  m_popup_timeout->setValue(QVariant(2));
  m_popup_timeout->setInfo(buffer);
  lt_groupBox2->addWidget(m_popup_timeout);
  // [General] mouse_wheel_actions
  buffer = tr("enable/disable Mousewheel action for Input edit");
  m_mouseWheel = new BoolBox(this);
  m_mouseWheel->setObjectName("mouse_wheel_actions");
  m_mouseWheel->setValue(false);
  m_mouseWheel->setWhatsThis(tr("If switched off, unintentional changing of "
                                "entries while scrolling is prevented."));
  m_mouseWheel->setInfo(buffer);
  lt_groupBox2->addWidget(m_mouseWheel);
  // [General] SqlQueryLimit
  buffer = tr("Limit Database results in tab main views!");
  m_queryLimit = new IntSpinBox(10, 15000, this);
  m_queryLimit->setObjectName("SqlQueryLimit");
  m_queryLimit->setValue(QVariant(1500));
  m_queryLimit->setWhatsThis(
      tr("Limits the output in the main views tables. Depending on the network "
         "and computer performance, prevents the application from freezing."));
  m_queryLimit->setInfo(buffer);
  lt_groupBox2->addWidget(m_queryLimit);
  buffer = tr(
      "Only Auto updates the main view if Table count is smaller than this!");
  m_queryAutoUpdate = new IntSpinBox(30, 100, this);
  m_queryAutoUpdate->setObjectName("SqlAutoUpdateCount");
  m_queryAutoUpdate->setValue(QVariant(50));
  m_queryAutoUpdate->setInfo(buffer);
  lt_groupBox2->addWidget(m_queryAutoUpdate);
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
  m_grouBox3->setTitle(tr("Font Settings") + " (Windows) ");
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
        w->setModified(false);
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
        w->setModified(false);
      }
    }
  }
}
